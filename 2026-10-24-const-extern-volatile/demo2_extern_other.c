/*
 * demo2（练习2）：extern 跨文件共享全局变量 —— 本文件负责“定义”
 *
 * 编译（两个 .c 必须一起交给 gcc）：
 *   gcc -Wall demo2_extern_main.c demo2_extern_other.c -o demo2 && ./demo2
 */
#include <stdio.h>

int g_counter = 42;          /* 定义：真正分配内存并赋初值（全工程只能有一处） */

void add_one(void)
{
    g_counter++;             /* 修改全局变量，另一个文件立刻能看到 */
    printf("add_one: g_counter = %d\n", g_counter);
}
