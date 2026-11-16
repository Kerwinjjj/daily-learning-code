# 多文件模块化工程（Day9）

> 以**代码和注释**为主，图表用于**比对**；英文缩写标注中文。

## 0. 一句话总结

| 文件 | 角色 | 放什么 | 绝不放什么 |
|---|---|---|---|
| `.h` | 对外接口清单 | 宏、`typedef`/`struct`、函数**声明**、`extern` 变量**声明** | 函数体、变量**定义** |
| `.c` | 内部实现 | 函数体、变量**定义**、`static` 私有变量 | —— |
| `main.c` | 使用者 | 只 `#include "test.h"` 就能调用 | —— |

## 1. 为什么要拆文件

`#include` 的本质是**文本复制**：预处理器把那个文件的内容原样粘到包含它的位置。所以头文件里写什么，就会被复制到每个包含它的 `.c` 各一份。

这解释了两件事：

- 为什么头文件里**定义变量**会报重复定义（被多个 `.c` 复制，就多份内存）；
- 为什么**保护宏**有用（`#include` 两次，就是粘两次）。

## 2. 声明 vs 定义（全篇核心）

| | 声明（declaration） | 定义（definition） |
|---|---|---|
| 变量 | `extern int g;` | `int g;` 或 `int g = 0;` |
| 函数 | `void f(void);`（分号结尾） | `void f(void) { ... }`（有花括号） |
| 本质 | 只描述名字与类型 | **真正分配内存 / 给出实现** |
| 能否重复 | 可以重复多次 | **全局范围内只能一处** |

判断技巧：**看它有没有占内存、有没有给出实现**。没有函数体就不是定义；不分配内存就不是定义。

## 3. 三文件工程

### test.h —— 只有声明

```c
#ifndef TEST_H
#define TEST_H
#include <stdint.h>
#define MAX_SAMPLES 8
typedef struct { uint8_t id; uint16_t value; } Sample_t;

void     sample_init(void);        /* 函数声明：分号结尾 */
int      sample_add(uint16_t v);
uint16_t sample_avg(void);
int      sample_count(void);

extern int g_sample_total;         /* 变量声明：extern */
#endif
```

### test.c —— 实现 + 定义

```c
#include "test.h"

static uint16_t s_buf[MAX_SAMPLES];  /* static：只在本文件可见，外部调不到 */
static int      s_count = 0;

int g_sample_total = 0;              /* 唯一定义，分配内存 */

void sample_init(void) { s_count = 0; g_sample_total = 0; }

int sample_add(uint16_t v)
{
    if (s_count >= MAX_SAMPLES) { return -1; }
    s_buf[s_count++] = v;
    g_sample_total += v;
    return 0;
}

uint16_t sample_avg(void)
{
    if (s_count == 0) { return 0; }
    return (uint16_t)(g_sample_total / s_count);
}

int sample_count(void) { return s_count; }
```

### main.c —— 只认识头文件

```c
#include <stdio.h>
#include "test.h"

int main(void)
{
    sample_init();
    uint16_t data[] = { 10, 20, 30, 40 };
    for (int i = 0; i < 4; i++) { sample_add(data[i]); }

    printf("count = %d\n", sample_count());
    printf("avg   = %u\n", sample_avg());
    printf("total = %d\n", g_sample_total);
    return 0;
}
```

### 编译命令

```bash
gcc -Wall main.c test.c -o demo      # 多个 .c 一起编，不编译 .h
```

**实测输出：**

```
count = 4
avg   = 25
total = 100
module = sample, max samples = 8
add #5 -> 0
add #6 -> 0
add #7 -> 0
add #8 -> 0
add #9 -> -1
count = 8
```

验算：`10+20+30+40 = 100`，`100/4 = 25`；上限 8，塞到第 9 个开始返回 `-1`。

`static` 的收益：`s_buf`、`s_count` 是模块内部细节，`main.c` 里根本访问不到。对外只暴露接口，内部随便改。

## 4. 保护宏做了什么

```c
#ifndef TEST_H      /* TEST_H 还没定义 → 说明第一次进来 */
#define TEST_H      /* 立刻定义它 */
...内容...
#endif
```

`gcc -E` 预处理后的实证（同一个头文件包含两次）：

| 情况 | 预处理后 `Point_t` 出现次数 |
|---|---|
| 无保护宏 | **2** |
| 有保护宏 | **1** |

保护宏就是把"第二次及以后的粘贴"变成空。

## 5. 两个坑的真实报错

### 坑① 头文件里定义变量 → 链接器报错

```c
/* bad.h 里写了： */
int g_counter = 0;      /* 这是定义，不是声明 */
```

错误原文：

```
/usr/bin/ld: (.bss+0x0): multiple definition of `g_counter'; (.bss+0x0): first defined here
collect2: error: ld returned 1 exit status
```

两个细节：

- 报错来自 `/usr/bin/ld`（**链接器**），不是编译器 —— 说明每个 `.c` 单独编译都过了，链接阶段才发现两份；
- `.bss` 段是零初始化全局变量所在，说明两边都真分配了内存。

修法：

```c
/* 头文件 */ extern int g_counter;      /* 只声明 */
/* 某个 .c */ int g_counter = 0;        /* 只定义一次 */
```

### 坑② 没有保护宏，重复包含

```
noguard.h:5:3: error: conflicting types for 'Point_t'; have 'struct <anonymous>'
note: previous declaration of 'Point_t' with type 'Point_t'
```

报错来自**编译器**，行号直指头文件第 5 行 —— 重复定义发生在同一个编译单元内部。

## 6. 三类报错对照

| 现象 | 谁报错 | 原因 | 修法 |
|---|---|---|---|
| `multiple definition of 'xxx'` | **链接器 ld** | 头文件里**定义**了变量，被多个 `.c` 各自复制 | 头文件改 `extern`，`.c` 里定义一次 |
| `conflicting types` / `redefinition` | **编译器** | 头文件没保护宏，被包含两次 | 加 `#ifndef / #define / #endif` |
| `undefined reference to 'xxx'` | 链接器 | 只声明没定义，或编译时漏了 `.c` | 补实现，或把 `.c` 加进编译命令 |

## 7. 口述题答法

**保护宏作用？** 防止同一个头文件被重复包含，避免 `struct`/`typedef`/宏重复定义。原理是条件编译：第一次进入时定义标记宏，之后条件不成立，整段跳过。

**声明和定义区别？** 声明只描述名字与类型，可重复；定义分配内存或给出函数体，全局只能一处。

**为什么头文件不能定义变量？** `#include` 是文本复制，头文件被 N 个 `.c` 包含就产生 N 份定义，链接时报 multiple definition。共享变量要头文件 `extern` 声明、一个 `.c` 定义。

## 8. 工程文件

| 文件 | 内容 |
|---|---|
| `test.h` | 保护宏、`MAX_SAMPLES`/`MODULE_NAME` 宏、`Sample_t` 类型、4 个函数声明、`extern` 变量声明 |
| `test.c` | 2 个 `static` 私有变量、1 个全局变量定义、4 个函数实现 |
| `main.c` | 调用接口、验证缓冲区上限行为 |
