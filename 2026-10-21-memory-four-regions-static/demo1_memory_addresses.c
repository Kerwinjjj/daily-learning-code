/*
 * demo1：打印不同类型变量的地址，观察各内存分区地址高低差异
 *
 * 观察点：
 *   1) 全局变量 g_val / 静态全局变量 g_static / 局部静态变量 s
 *      —— 地址相邻成组，说明它们都在"全局/静态区（数据段）"
 *   2) 普通局部变量 a —— 在栈上，地址明显更高，且每次运行都可能变化（ASLR）
 *   3) 字符串常量 "hello" —— 在只读常量段，与数据段相邻但不同区
 *
 * 编译：gcc demo1_memory_addresses.c -o demo1 && ./demo1
 */
#include <stdio.h>

int g_val = 10;              /* 全局变量：数据段 .data（已初始化） */
static int g_static = 20;    /* 静态全局变量：数据段，仅本文件可见 */

int main(void)
{
    int a = 1;               /* 普通局部变量：栈，函数返回自动释放 */
    static int s = 2;        /* 局部静态变量：数据段，只初始化一次 */
    char *str = "hello";     /* str 指针本身在栈；"hello" 本体在只读常量段 */

    /* %p 打印地址；& 取变量地址；str 本身就是字符串常量的首地址 */
    printf("g_val: %p\n", &g_val);
    printf("g_static: %p\n", &g_static);
    printf("a(stack): %p\n", &a);
    printf("s(static local): %p\n", &s);
    printf("str literal: %p\n", str);

    return 0;
}
