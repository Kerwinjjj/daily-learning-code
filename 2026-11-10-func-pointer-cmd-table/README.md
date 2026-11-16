# 函数指针、回调函数、命令表（Day8）

> 以**代码和注释**为主，图表用于**比对**；英文缩写标注中文。

## 0. 一句话总结

| 概念 | 一句话 |
|---|---|
| 函数指针 | 存放**函数入口地址**的变量，指向代码段 |
| 回调函数 | 把自己的函数交给别人，由别人在合适时机调用 |
| 函数指针数组 | 同签名的函数排成表，用下标或名字查表调用（命令表） |
| `typedef` | 给类型起别名，让函数指针的声明变短、可读 |

## 1. 函数指针的本质

| | 普通指针 | 函数指针 |
|---|---|---|
| 存什么 | 数据 / 变量的地址 | 函数入口地址 |
| 指向的内存 | 栈、堆、全局区 | **代码段**（只读） |
| 声明要点 | 类型 `*名字` | 必须写**返回值 + 参数表** |

**声明读法（右左法则）：**

```c
int (*p)(int, int);
/*  ↑   ↑  ↑
    |   |  └─ 参数表：两个 int
    |   └─ 名字先和 * 结合 → 这是指针
    └─ 返回值：int
 */
```

对比一个容易混的写法：

```c
int *f(int);       /* f 是函数，接收 int，返回 int* —— 普通函数声明 */
int (*f)(int);     /* f 是指针，指向"接收 int、返回 int"的函数 */
```

区分方法：看**名字先和 `*` 结合还是和 `(` 结合**。

## 2. demo1：同一接口，三种算法

```c
typedef int (*CalcFunc)(int, int);      /* 这一类函数指针的类型名 */

int calc(int a, int b, CalcFunc fn)
{
    if (fn == NULL) return 0;           /* 函数指针可能是空指针，先判 */
    return fn(a, b);                     /* 通过指针调用 */
}

calc(10, 5, add);    /* 传 add → 加法 */
calc(10, 5, sub);    /* 传 sub → 减法 */
calc(10, 5, mul);    /* 传 mul → 乘法 */
```

**实测输出：**

```
p(3, 4)    = 7
(*p)(3, 4) = 7
&add = 0x00007FF6C0441760
add  = 0x00007FF6C0441760      （两者完全相同；具体地址每次运行都会变）
calc(10,5,add) = 15
calc(10,5,sub) = 5
calc(10,5,mul) = 50
```

三个要点：

- `p(3,4)` 与 `(*p)(3,4)` **完全等价**，编译器自动解引用；
- `add` 和 `&add` 是**同一个地址**，函数名本身即地址；
- `calc` 不知道具体在算加法还是乘法，**行为由调用者传进来**，这就是解耦。

## 3. demo2：命令表（串口指令分发）

### 最简版：下标即命令号

```c
typedef void (*CmdHandler)(void);
CmdHandler cmd_table[] = { led_on, led_off };

cmd_table[0]();      /* 0 号命令 → led_on */
cmd_table[1]();      /* 1 号命令 → led_off */
```

### 工程版：字符串匹配

```c
typedef struct {
    const char *name;               /* 指令字符串 */
    CmdHandler  handler;            /* 对应的处理函数 */
} CmdEntry;

static const CmdEntry cmd_table[] = {
    { "led on",     cmd_led_on },
    { "led off",    cmd_led_off },
    { "motor run",  cmd_motor_run },
    { "motor stop", cmd_motor_stop },
    { "status",     cmd_status },
};

static int dispatch(const char *cmd)
{
    for (size_t i = 0; i < CMD_COUNT; i++) {
        if (strcmp(cmd, cmd_table[i].name) == 0) {
            cmd_table[i].handler();
            return 0;
        }
    }
    return -1;                      /* 未知指令走错误分支 */
}
```

**实测输出：**

```
收到: led on -> [执行] 开灯
收到: motor run -> [执行] 电机启动
收到: status -> [执行] 打印状态
收到: led off -> [执行] 关灯
收到: foo -> [未知指令] foo
--- 纯数组下标调用 ---
[执行] 开灯
[执行] 关灯
```

### 表驱动 vs if-else 链

| 写法 | 加一条新指令 | 代码规模 | 可测试性 |
|---|---|---|---|
| `if-else if` 链 | 中间插一段分支 | 越写越长，每次改同一段 | 只能整体测 |
| 命令表 | **表里加一行** | 由表长度决定，解析逻辑不变 | 每个 handler 单独测 |

表长用 `sizeof(cmd_table) / sizeof(cmd_table[0])` 算，加条目不用改数字。

## 4. typedef 拆解（demo3）

`typedef` 就是**给类型起别名**，不创建变量、不分配内存。名字必须写在"变量名原本该在的位置"：

| 想取别名的类型 | 它的变量声明长这样 | 名字位置 | 所以别名这么写 |
|---|---|---|---|
| `int` | `int x;` | 末尾 | `typedef int MyInt;` |
| `unsigned char` | `unsigned char x;` | 末尾 | `typedef unsigned char u8;` |
| `struct {...}` | `struct {...} x;` | 末尾 | `typedef struct {...} Device_t;` |
| 函数指针 | `int (*x)(int, int);` | **在 `(*x)` 里** | `typedef int (*CalcFunc)(int, int);` |

**实测（demo3）：**

```
sizeof(raw) = 8, sizeof(ali) = 8, sizeof(CalcFunc) = 8
raw(3,4) = 7, ali(3,4) = 7
calc_raw(10,5,add)   = 15
calc_alias(10,5,add) = 15
table[0](5,6) = 11
```

### `CalcFunc fn` 里那两个东西分别是什么

```c
int calc(int a, int b, CalcFunc fn)
{
    return fn(a, b);
}
```

| 位置 | 名字 | 层级 | 是什么 |
|---|---|---|---|
| `CalcFunc` | 类型名 | 类型 | "一种函数指针"，可以重复用在很多地方 |
| `fn` | 参数名 | 变量 | 这个参数的名字，只在函数内有效 |

等价于 `int (*fn)(int, int)`，只是把类型那部分换成了短名。`fn` 必须存在，因为函数体要用它调用传进来的函数。

## 5. 四个坑

| 坑 | 后果 | 防法 |
|---|---|---|
| 函数指针未判空就调 | 跳到地址 0，崩溃 | 调用前 `if (fn)` |
| 签名不匹配（参数/返回值不同） | 参数错乱、栈被破坏，**编译器可能只给 warning** | 用 `typedef` 统一类型 |
| 声明写错 `int *f(int)` | 变成普通函数声明 | 看名字先和 `*` 还是 `(` 结合 |
| 表下标越界 | 调到无关函数，随机行为 | 下标来自外部时先做范围检查 |

## 6. 口述题答法

**函数指针用途（四层）：**

1. **解耦**：调用者只依赖函数签名，不依赖具体实现；
2. **可注册/回调**：驱动层向上层注册回调，事件发生时反向调用；
3. **表驱动**：同签名函数组成表，用下标或字符串查表调用，替代长 `if-else`；
4. **跨模块传递行为**：把"做什么"当参数传进去（`qsort` 比较函数、RTOS 任务入口）。

**ESP32 小车项目的五个落点：**

| 落点 | 怎么用 |
|---|---|
| 串口指令解析 | 上位机发 `led on` / `motor run` → 命令表查表分发 |
| 中断回调 | `gpio_isr_handler_add(PIN, encoder_isr, NULL)`，中断发生时框架调你的函数 |
| 状态机 | 待机/手动/自动/避障，每个状态一个处理函数，用当前状态做下标 |
| FreeRTOS 任务 | `xTaskCreate(task_motor_ctrl, ...)` 第一个参数就是函数指针 |
| 传感器统一接口 | 各型号实现 `init/read`，上层只拿一套函数指针结构体 |

项目亮点写法：

> 基于函数指针命令表实现串口指令分发：指令与处理函数以 `{name, handler}` 注册，新增指令只需在表里加一行，解析逻辑零改动；未知指令统一走错误分支。

## 7. Demo 速览

| Demo | 内容 | 关键输出 |
|---|---|---|
| demo1_func_ptr.c | 函数指针基础 + 统一接口 | `p(3,4)=7`，`add` 与 `&add` 同址，`calc` 得 15/5/50 |
| demo2_cmd_table.c | 命令表（结构体版 + 纯数组版） | 5 条指令正确分发，未知指令走错误分支 |
| demo3_typedef_alias.c | typedef 三种写法等价性 | 三个 `sizeof` 都是 8，`calc_raw` 与 `calc_alias` 同结果 |

```bash
gcc -Wall demo1_func_ptr.c -o demo1 && ./demo1
gcc -Wall demo2_cmd_table.c -o demo2 && ./demo2
gcc -Wall demo3_typedef_alias.c -o demo3 && ./demo3
```
