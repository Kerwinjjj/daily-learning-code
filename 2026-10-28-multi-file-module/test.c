/*
 * test.c —— 模块的"实现文件"
 *
 * 这里放：函数体、变量定义（真正分配内存的地方）
 * 只在本文件内部用的变量，一律加 static，外部看不见
 */
#include "test.h"

/* static 修饰的全局变量：作用域只在本文件，链接器看不到它 */
static uint16_t s_buf[MAX_SAMPLES];   /* 采样缓冲区 */
static int      s_count = 0;          /* 当前存了几个 */

/* 真正"定义"全局变量：分配内存的地方，只能有一处 */
int g_sample_total = 0;

void sample_init(void)
{
    s_count = 0;
    g_sample_total = 0;
}

int sample_add(uint16_t v)
{
    if (s_count >= MAX_SAMPLES) {     /* 缓冲区满了就拒绝 */
        return -1;
    }
    s_buf[s_count++] = v;
    g_sample_total += v;
    return 0;
}

uint16_t sample_avg(void)
{
    if (s_count == 0) {               /* 除零保护 */
        return 0;
    }
    return (uint16_t)(g_sample_total / s_count);
}

int sample_count(void)
{
    return s_count;
}
