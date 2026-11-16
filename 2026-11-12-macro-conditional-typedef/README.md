# 宏定义、条件编译、typedef（Day10）

> 以**代码和注释**为主，图表用于**比对**；英文缩写标注中文。

## 0. 一句话总结

| 工具 | 一句话 |
|---|---|
| `#define` | 预处理阶段的**文本替换**，编译器看不到宏本身 |
| `inline` | 编译器认识的函数，可内联展开，参数只求值一次 |
| `typedef` | 给**类型**起别名，有类型检查、有作用域 |
| `#ifdef / #endif` | 条件编译，决定哪些代码进入编译产物 |

## 1. 宏的本质

`#define` 发生在**预处理阶段**（编译之前），做的就是文本替换。所以宏：

- 不检查类型，`SQUARE("abc")` 也能通过预处理，错误留到后面才爆；
- 不检查语法细节，只看括号配对；
- 没有作用域，从定义处到文件末尾一直有效。

## 2. demo1：两个必踩的坑（含真实展开）

**运行输出：**

```
SQUARE_BAD(1+2) = 5     （正确值 9）
SQUARE_OK (1+2) = 9     （正确值 9）
MAX_BAD(i++, 3) = 6, 之后 i = 7    （i 应为 6）
max_inline(j++,3) = 5, 之后 j = 6  （j 应为 6）
```

**`gcc -E` 看到的真实展开：**

```c
printf(..., 1 + 2 * 1 + 2);                    /* SQUARE_BAD(1+2) → 5  */
printf(..., ((1 + 2) * (1 + 2)));              /* SQUARE_OK(1+2)  → 9  */
int m = ((i++) > (3) ? (i++) : (3));           /* MAX_BAD(i++,3) → i 加两次 */
```

| 写法 | 展开后 | 结果 |
|---|---|---|
| `#define SQUARE_BAD(x) x * x` | `1 + 2 * 1 + 2` | **5**（应为 9） |
| `#define SQUARE_OK(x) ((x) * (x))` | `((1 + 2) * (1 + 2))` | 9 |
| `#define MAX_BAD(a,b) ((a)>(b)?(a):(b))` | `((i++) > (3) ? (i++) : (3))` | i 从 5 变 **7** |

关键结论：**加括号只能修好优先级，修不好"重复求值"。** `MAX_BAD` 的括号一个不少，`i++` 仍被求值两次，因为宏是文本替换，每个 `a` 的位置都被替换成 `i++`。

这种 bug 在嵌入式里极难查，例如 `MAX(adc_read(), 100)` 会把传感器读两次。

## 3. 宏 vs inline 函数

| | `#define` 宏 | `static inline` 函数 |
|---|---|---|
| 本质 | 预处理文本替换 | 真函数，编译器可内联展开 |
| 类型检查 | 无 | 有（传错类型直接报错） |
| 参数求值次数 | **可能多次** | 只求值一次 |
| 调试 | 断点打不进，看不到变量 | 能单步、能看变量 |
| 代码体积 | 每处展开一份 | 内联多份，不内联一份 |
| 作用域 | 文件内，可 `#undef` | 正常作用域 |

实测对照：`MAX_BAD(i++,3)` 之后 `i = 7`；`max_inline(j++,3)` 之后 `j = 6`。**同样一次 `i++`，宏让它加了两次。**

## 4. typedef 与 #define 的区别（demo2）

**实测输出：**

```
p = (3, 4)
PTR_INT  : sizeof(a)=8, sizeof(b)=4
IntPtr   : sizeof(c)=8, sizeof(d)=8
ops[0](7,3) = 10
ops[1](7,3) = 4
```

原因：

```c
#define PTR_INT int*      /* 文本替换 */
PTR_INT a, b;             /* 展开成 int* a, b; → a 是指针，b 是 int */

typedef int *IntPtr;      /* 编译器认的类型名 */
IntPtr c, d;              /* c、d 都是指针 */
```

`int* a, b;` 里 `*` 只修饰 `a`，所以 `b` 退化成 `int`（4 字节）。typedef 是真类型，两个都是 8 字节。

另外两个 typedef 用法：

```c
typedef struct { int x; int y; } Point_t;     /* 省掉每次写 struct */
typedef int (*CalcFunc)(int, int);             /* 函数指针别名 */
CalcFunc ops[] = { add, sub };                 /* 一个数组装两种运算 */
```

## 5. 条件编译（demo3）

```c
#ifdef HW_BOARD_A
  #define BOARD_NAME "STM32F407"
  #define LED_PIN    0
  #define UART_BAUD  115200
#elif defined(HW_BOARD_B)
  #define BOARD_NAME "ESP32"
  #define LED_PIN    2
  #define UART_BAUD  921600
#else
  #error "必须指定硬件版本：-DHW_BOARD_A 或 -DHW_BOARD_B"
#endif
```

**两块板子的实测输出：**

| 板子 A（`-DHW_BOARD_A`） | 板子 B（`-DHW_BOARD_B`） |
|---|---|
| `board = STM32F407` | `board = ESP32` |
| `led pin = 0` | `led pin = 2` |
| `baud = 115200` | `baud = 921600` |

不指定时编译直接失败：

```
demo3_conditional.c:17:4: error: #error "必须指定硬件版本：-DHW_BOARD_A 或 -DHW_BOARD_B"
```

`#error` 是一道**编译期保险**：少传宏就编不过，不会出现"编成功但引脚配错、板子跑飞"。

### 条件编译的四个典型场景

| 场景 | 做法 |
|---|---|
| 多硬件版本共用一套代码 | `#ifdef BOARD_A` / `#elif BOARD_B` 选引脚、波特率、外设 |
| 调试版 vs 发布版 | `#ifdef DEBUG` 打开串口打印，发布版整个不编译 |
| 平台差异 | Windows / Linux / ARM 各写一套实现 |
| 功能开关 | `#ifdef ENABLE_OLED` 决定要不要编进固件（省 Flash） |

## 6. 口述题答法

**宏和 inline 函数区别？** 宏是预处理文本替换，没有类型检查、参数可能被求值多次、调试困难，但可跨类型"通用"；inline 函数是真函数，参数只求值一次、有类型检查、能调试，编译器决定是否内联以兼顾性能与体积。原则：**计算逻辑用 inline 函数，只有必须做编译期替换的（条件编译、常量、字符串拼接）才用宏。**

**条件编译什么场景用？** ① 同一套代码适配不同硬件版本（引脚、波特率、外设不同）；② 调试与发布版切换；③ 跨平台/跨编译器差异处理；④ 功能模块按需裁剪，减小固件体积。

## 7. Demo 速览

| Demo | 内容 | 关键输出 |
|---|---|---|
| demo1_macro_trap.c | 缺括号 + 自增副作用 + inline 对照 | `SQUARE_BAD(1+2)=5`；`MAX_BAD(i++,3)` 后 `i=7` |
| demo2_typedef_alias.c | 结构体/指针/函数指针别名，`#define` 对比 | `sizeof(a)=8, sizeof(b)=4` vs `8, 8` |
| demo3_conditional.c | 条件编译切两块板子 | STM32F407/115200 vs ESP32/921600，缺宏时 `#error` |

```bash
gcc -Wall demo1_macro_trap.c -o demo1 && ./demo1
gcc -Wall demo2_typedef_alias.c -o demo2 && ./demo2
gcc -Wall -DHW_BOARD_A demo3_conditional.c -o demo3a && ./demo3a
gcc -Wall -DHW_BOARD_B demo3_conditional.c -o demo3b && ./demo3b
gcc -E demo1_macro_trap.c | grep "int m"        # 看宏的真实展开
```
