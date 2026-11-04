# C 存储与链接属性：const / extern / volatile（Day4 · MCU 面试必问）

> 以**代码和注释**为主，图表用于**比对**；英文缩写标注中文。MCU（微控制器）面试必问 volatile。

## 0. 一句话总结

| 关键字 | 管什么 | 一句话 |
|---|---|---|
| `const`（常量限定符） | 编译期只读约束 | 这个对象不允许改，改了编译报错 |
| `extern`（外部声明） | 跨文件可见性 | 变量/函数在**别的 .c 里**，这里只是声明 |
| `volatile`（易失的） | 禁止编译器优化访问 | 每次都要真读真写内存，值可能被硬件/中断改掉 |

## 1. const：三种写法——"离谁近谁不可改"

把 `const` 当成一张**贴在谁身上就锁住谁**的标签，看它夹在 `*` 的哪边：

| 写法 | const 的位置 | 谁不可改 | 记忆口诀 | 实测编译错误 |
|---|---|---|---|---|
| `const int *p` | `*` 左边 | **指向的数据**（`*p`） | 离 `*p` 近 | `assignment of read-only location '*p'` |
| `int *const p` | `*` 右边 | **指针本身**（`p` 的指向） | 离 `p` 近 | `assignment of read-only variable 'p'` |
| `const int *const p` | 两边都有 | 数据和指针**都不能改** | 两个都锁 | 上面两条错误各报一次 |

### 代码（demo1）

```c
#include <stdio.h>

int main(void)
{
    int a = 1, b = 2;

    const int *p1 = &a;        /* 指向的数据是只读的（*p1 不能改） */
    int *const p2 = &a;        /* 指针本身是只读的（p2 的指向不能改） */
    const int *const p3 = &a;  /* 数据和指针都只读 */

    p1 = &b;                   /* 合法：能改“指向”，不能改 *p1 */
    printf("p1 = &b -> *p1 = %d\n", *p1);   /* 2 */

    *p2 = 100;                 /* 合法：能改数据，不能改“指向” */
    printf("a = %d\n", *p2);                /* 100 */

    printf("p3 = %d\n", *p3);               /* 100，p3 只能读 */
    return 0;
}
```

**实测输出：**

```
p1 = &b -> *p1 = 2
a = 100
p3 = 100
```

### 非法修改会报什么错（实测）

| 非法操作 | 编译错误 |
|---|---|
| `*p1 = 10;` | `assignment of read-only location '*p1'` |
| `p2 = &b;` | `assignment of read-only variable 'p2'` |
| `*p3 = 10;` 和 `p3 = &b;` | 上面两条错误各报一次 |

### 固件里怎么用

- 只读配置表、寄存器地址表用 `const`，放 Flash（闪存）省 SRAM（静态随机存取存储器）；
- 函数只读形参加 `const`（`const uint8_t *buf`），防手滑改入参，也让编译器知道你承诺不改。

## 2. extern：打通多个 .c 文件

### 先分清"声明"和"定义"

| 概念 | 写法 | 是否分配内存 | 全工程能写几次 |
|---|---|---|---|
| 定义 | `int g_counter = 42;` | **分配** | 只能**一处** |
| 声明 | `extern int g_counter;` | **不分配** | 可以有任意多处 |

### 代码（demo2：两个 .c）

```c
/* demo2_extern_other.c —— 定义：真正分配内存并给初值 */
#include <stdio.h>

int g_counter = 42;
void add_one(void)
{
    g_counter++;             /* 修改全局变量，另一个文件立刻能看到 */
    printf("add_one: g_counter = %d\n", g_counter);
}
```

```c
/* demo2_extern_main.c —— 声明：不分配内存，只告诉编译器“别处有” */
#include <stdio.h>

extern int g_counter;        /* 声明外部全局变量 */
extern void add_one(void);   /* 函数声明默认就是 extern，可省略 */

int main(void)
{
    printf("before: g_counter = %d\n", g_counter);
    add_one();
    add_one();
    printf("after : g_counter = %d\n", g_counter);
    return 0;
}
```

**编译（两个 .c 必须一起交给 gcc）：**

```bash
gcc -Wall demo2_extern_main.c demo2_extern_other.c -o demo2 && ./demo2
```

**实测输出：**

```
before: g_counter = 42
add_one: g_counter = 43
add_one: g_counter = 44
after : g_counter = 44
```

### STM32 工程里的典型用法

- 头文件里写 `extern` 声明，某一个 .c 里定义，其他文件 `#include` 头文件就能用；
- 外设句柄、全局状态机变量都这么共享（也是后面多文件工程的基础）。

**常见坑：** 两个 .c 都写 `int g_counter = 0;` → 现代 gcc（默认 `-fno-common`）报 `multiple definition`（重复定义）。

## 3. volatile（重点）

### 3.1 编译器优化是什么

编译器默认假设：**这个变量只有当前程序在改，别的因素（硬件、中断）不会动它。** 于是它会做三件事：

| 优化动作 | 表现 | 单片机上出什么事 |
|---|---|---|
| 读到寄存器后复用 | 变量读一次进 CPU 寄存器，之后不再访存 | 中断改了内存，主循环还看旧副本 |
| 删除重复读 | 循环里反复读同一变量被删掉 | `while (!flag)` 死循环 |
| 合并写 | 连续写同一地址只保留最后一次 | 外设寄存器配置被吞掉 |

### 3.2 实测：不加 volatile 会怎样（demo3）

```c
#include <stdio.h>
#include <signal.h>
#include <unistd.h>

int flag = 0;                  /* 不加 volatile：-O2 下会被缓存进寄存器 */
void on_alarm(int sig) { (void)sig; flag = 1; }   /* 模拟 ISR（中断服务程序） */

int main(void)
{
    signal(SIGALRM, on_alarm); /* 注册“闹钟信号 -> on_alarm” */
    alarm(1);                  /* 1 秒后“中断”到来 */
    while (!flag) { }          /* 主循环等待标志 */
    printf("escaped, flag = %d\n", flag);
    return 0;
}
```

**`gcc -O2` 编译后运行：程序卡在 `while (!flag)`，永远读不到 flag 变成 1（实测 3 秒超时，需 Ctrl+C 结束）。**

**汇编给出的"罪证"（-O2）：**

```
; 不加 volatile：进循环前只读一次内存，之后纯空转
    movl    flag(%rip), %edx    ; 只读这一次
    testl   %edx, %edx
    jne     .L7
.L6:
    jmp     .L6                 ; 死循环，再也不访存
```

**加 volatile 后，同一段循环变成：**

```
; 加 volatile：每一圈都从内存重读
.L4:
    movl    flag(%rip), %eax    ; 每次循环都重新读内存
    testl   %eax, %eax
    je      .L4
```

**实测：加了之后 1 秒正常退出，输出 `escaped, flag = 1`。**

### 3.3 为什么你自己试可能"不复现"

| 编译选项 | 不加 volatile | 加 volatile |
|---|---|---|
| `-O0`（不优化） | 每圈读内存，正常退出 | 每圈读内存，正常退出 |
| `-O2`（优化） | 只读一次后空转 → **死循环**（实测） | 每圈重读，正常退出 |

**必须加 `-O2`**（真实工程的优化级别）才能看到差别；`-O0` 下编译器根本不优化，看不出问题。

### 3.4 外设寄存器：`*(volatile uint32_t *)0x40021000 = 1;` 三步拆

| 步骤 | 代码片段 | 含义 | 类比 |
|---|---|---|---|
| ① | `0x40021000` | 一个固定地址 | 门牌号 |
| ② | `(volatile uint32_t *)` | 把地址强转成“指向 32 位无符号整数的指针” | 认定这个门牌上住着一个 32 位整数 |
| ③ | 前面的 `*` + 赋值 | 解引用：往那一格写值 | 开门进屋，把东西放进格子 |

- 地址不是编的，来自芯片手册的 memory map（存储器映射）：GPIOA 的寄存器就在 `0x4002 0000` 一带；
- 写这一格 = 让硬件引脚电平变化（点灯、拉高片选……）；
- 必须 volatile：这是"往硬件里写"，编译器不知道地址背后连着电路，可能合并/省略写。

### 3.5 单片机里必须加 volatile 的三类对象

```c
/* 1. 外设寄存器（固定地址，值由硬件改） */
#define RCC_CR  (*(volatile uint32_t *)0x40021000)
RCC_CR |= (1u << 16);      /* 不加 volatile：读-改-写可能被合并/省略，寄存器配置失效 */

/* 2. 中断与主循环共享的标志（最经典） */
static volatile uint8_t g_rx_done = 0;   /* ISR 里置 1，主循环等它 */
void USART1_IRQHandler(void) { g_rx_done = 1; }
while (!g_rx_done) { }                    /* 不加 volatile：-O2 下可能死等 */

/* 3. 主循环与中断共享的计数/缓冲下标（DMA 直接存储器访问 缓冲区同理） */
static volatile uint32_t g_tick = 0;
void SysTick_Handler(void) { g_tick++; }  /* 主循环读 g_tick 也必须 volatile */
```

### 3.6 边界：volatile 不保证原子性

`g_tick++` 是"读 → 加 → 写"三步，中断可能插在中间，计数会丢；volatile 只保证"每次真访问内存"，**不保证原子性、不保证内存顺序**。要原子请用关中断（`__disable_irq()`）或原子指令；多核/多线程还要内存屏障。volatile 不是线程同步工具。

## 4. 面试题：volatile 作用？什么时候必须加？

**按四层答（先作用 → 再机制 → 再场景 → 最后边界）：**

1. **作用**：告诉编译器这个对象的值可能在程序控制之外被改变（硬件、中断、其他执行流），因此**每次访问都必须真正访问内存**，禁止把值缓存在寄存器、禁止删除重复读、禁止合并写；
2. **机制**：不加时编译器只读一次进寄存器，循环里一直用副本（-O2 汇编就是 `jmp 自己` 空转）；加了之后每圈 `movl` 重读；
3. **必须加的场景**：① 外设寄存器（`*(volatile uint32_t *)0x40021000`）；② 中断（ISR）和主循环共享的标志变量；③ DMA（直接存储器访问）缓冲区 / 共享缓冲下标；
4. **边界**：volatile 只管可见性，不管原子性和内存顺序——多任务共享数据还要加锁/关中断。

**一句话记忆：** "**中断会改、硬件会改 → 就得 volatile**；volatile 让编译器每次都去内存取最新值，而不是拿旧副本。"

## 5. 三组关键字对比速查

| 关键字 | 作用域 | 编译器/链接器行为 | 嵌入式典型场景 |
|---|---|---|---|
| `const` | 编译期约束 | 禁止通过该名字修改；可能放只读段 | 只读形参、配置表、寄存器地址表 |
| `extern` | 链接期 | 声明"别处有"，不分配内存 | 跨文件全局变量、多文件工程共享状态 |
| `volatile` | 每次访问 | 强制访存，禁止缓存/删除/合并 | 外设寄存器、中断共享标志、DMA 缓冲 |

## 6. 常见误区速查

| 误区 | 真相 |
|---|---|
| "const 变量一定在只读段" | 全局 `const` 常在 `.rodata`（只读数据段）；局部 `const` 一般仍在栈上，只是编译器禁止通过该名字修改 |
| "const 是运行时保证" | 是编译期约束；用指针强转仍可改，但那是未定义行为，别干 |
| "extern 是定义" | 是**声明**，不分配内存；定义才分配 |
| "两个 .c 都定义同一全局变量没事" | 现代 gcc 默认 `-fno-common`，直接报 `multiple definition` |
| "加了 volatile 就线程安全" | 不保证原子性、不保证内存顺序；`g_tick++` 仍会丢计数 |
| "volatile 让程序更快" | 相反：禁止缓存，每次访存，速度变慢，用正确性换的 |
| "Windows 上也能跑这段中断演示" | 不能：`SIGALRM`/`alarm` 是 Linux/POSIX 独有，MinGW 实测报 `'SIGALRM' undeclared` |

## 7. Demo 速览

| Demo | 内容 | 关键输出 |
|---|---|---|
| demo1_const_three_writes.c | const 三写法 + 合法/非法修改 | `p1 = &b -> *p1 = 2`、`a = 100` |
| demo2_extern_main.c + demo2_extern_other.c | extern 跨文件共享全局变量 | before 42 → after 44 |
| demo3_volatile_flag.c | volatile 标志（模拟中断改标志） | 不加：死循环；加：`escaped, flag = 1` |

```bash
gcc -Wall demo1_const_three_writes.c -o demo1 && ./demo1
gcc -Wall demo2_extern_main.c demo2_extern_other.c -o demo2 && ./demo2
gcc -O2 demo3_volatile_flag.c -o demo3 && ./demo3    # 需 Linux / WSL2 Ubuntu 环境
```
