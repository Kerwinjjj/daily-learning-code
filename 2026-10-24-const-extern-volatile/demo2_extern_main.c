/*
 * demo2（练习2）：extern 跨文件共享全局变量 —— 本文件负责“声明并使用”
 *
 * 声明 vs 定义：
 *   extern int g_counter;  声明：不分配内存，只告诉编译器“别处有这个变量”
 *   int g_counter = 0;     定义：分配内存，全工程只能一次
 *
 * 编译：gcc -Wall demo2_extern_main.c demo2_extern_other.c -o demo2 && ./demo2
 */
#include <stdio.h>

extern int g_counter;        /* 声明外部全局变量（定义在 demo2_extern_other.c） */
extern void add_one(void);   /* 函数声明默认就是 extern，可省略 extern */

int main(void)
{
    printf("before: g_counter = %d\n", g_counter);   /* 42 */

    add_one();                                       /* 调用另一个 .c 里的函数修改 */
    add_one();

    printf("after : g_counter = %d\n", g_counter);   /* 44 */
    return 0;
}
