/*
 * demo2：static 局部静态变量——多次调用函数，观察值不会重置
 * static 局部变量存储在全局/静态区，只初始化一次，函数返回后值保留
 * 预期输出：count = 1 到 count = 5（而不是每次都输出 1）
 * 编译：gcc demo2_static_local_var.c -o demo2 && ./demo2
 */
#include <stdio.h>

void counter(void)
{
    static int count = 0;   /* 只初始化一次，之后的调用不再重新初始化 */
    count++;
    printf("count = %d\n", count);
}

int main(void)
{
    int i;
    for (i = 0; i < 5; i++) {
        counter();
    }
    return 0;
}
