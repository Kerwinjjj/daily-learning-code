/*
 * demo1：打印不同类型变量的地址，观察各内存分区地址高低差异
 * 全局变量 / 静态变量在全局区（数据段），局部变量在栈（高地址），字符串常量在只读段
 * 编译：gcc demo1_memory_addresses.c -o demo1 && ./demo1
 */
#include <stdio.h>

int g_val = 10;
static int g_static = 20;

int main(void)
{
    int a = 1;
    static int s = 2;
    char *str = "hello";

    printf("g_val: %p\n", &g_val);
    printf("g_static: %p\n", &g_static);
    printf("a(stack): %p\n", &a);
    printf("s(static local): %p\n", &s);
    printf("str literal: %p\n", str);

    return 0;
}
