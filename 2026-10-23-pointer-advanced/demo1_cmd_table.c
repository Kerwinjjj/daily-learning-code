/*
 * demo1（练习1）：指针数组——字符串命令表（固件命令行解析常用）
 *
 * 要点：
 *   1) commands 是"指针数组"：每个元素是 char*（指向一条命令字符串）
 *   2) 元素个数 = sizeof(commands) / sizeof(commands[0])（64 位下 32/8=4）
 *   3) 比"定长二维数组"省内存：字符串长短不一，只存指针不存整行
 *
 * 编译：gcc demo1_cmd_table.c -o demo1 && ./demo1
 */
#include <stdio.h>

char *commands[] = {"help", "version", "reset", "start"};  /* 指针数组：命令表 */

int main(void)
{
    int i;
    int n = sizeof(commands) / sizeof(commands[0]);  /* 元素个数 = 4 */

    for (i = 0; i < n; i++) {
        printf("cmd[%d] = %s\n", i, commands[i]);   /* %s 传指针即可 */
    }
    return 0;
}
