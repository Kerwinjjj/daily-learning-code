/*
 * main.c —— 使用模块的人
 *
 * 只需要 #include "test.h" 就能调用所有接口，
 * 完全不用关心 test.c 里怎么实现的（这就是模块化的意义）
 */
#include <stdio.h>
#include "test.h"

int main(void)
{
    sample_init();                    /* 初始化 */

    /* 塞几个采样值进去 */
    uint16_t data[] = { 10, 20, 30, 40 };
    for (int i = 0; i < 4; i++) {
        if (sample_add(data[i]) != 0) {
            printf("add failed at %d\n", i);
        }
    }

    printf("count = %d\n", sample_count());
    printf("avg   = %u\n", sample_avg());
    printf("total = %d\n", g_sample_total);      /* extern 声明的变量，直接用 */
    printf("module = %s, max samples = %d\n", MODULE_NAME, MAX_SAMPLES);

    /* 缓冲区塞满测试：MAX_SAMPLES = 8，现在已有 4 个 */
    for (int i = 0; i < 5; i++) {
        printf("add #%d -> %d\n", i + 5, sample_add(100));
    }
    printf("count = %d\n", sample_count());

    return 0;
}
