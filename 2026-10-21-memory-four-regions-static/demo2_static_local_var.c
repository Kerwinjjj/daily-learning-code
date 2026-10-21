/*
 * demo2：static 局部静态变量——多次调用函数，观察值不会重置
 *
 * 原理：
 *   static 局部变量存放在"全局/静态区（数据段）"，而不是栈；
 *   只在程序第一次进入函数时初始化一次，函数返回后值继续保留，
 *   所以每次调用都在上一次的基础上累加（count = 1 → 5）。
 *   如果去掉 static，count 会变成栈上的普通局部变量，
 *   每次调用都重新初始化为 0，输出就会一直是 count = 1。
 *
 * 编译：gcc demo2_static_local_var.c -o demo2 && ./demo2
 */
#include <stdio.h>

void counter(void)
{
    static int count = 0;   /* 局部静态变量：只初始化一次，跨调用保持值 */
    count++;                /* 每次调用自增：1, 2, 3, 4, 5 */
    printf("count = %d\n", count);
}

int main(void)
{
    int i;
    for (i = 0; i < 5; i++) {
        counter();          /* 连续调用 5 次，观察 count 是否重置 */
    }
    return 0;
}
