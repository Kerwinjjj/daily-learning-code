/*
 * demo3（重点）：volatile 演示 —— “中断”改了标志，主循环能不能立刻看到
 *
 * 这段代码用 Linux/POSIX 的“信号”模拟单片机中断：
 *   alarm(1) 到期 -> 发送 SIGALRM -> 执行 on_alarm（相当于 ISR 中断服务程序）
 *   while (!flag) 相当于单片机的 while(1) 主循环
 *
 * 注意：signal / alarm / SIGALRM 是 Linux/POSIX 才有的。
 *       Windows(MinGW) 编译会报 'SIGALRM' undeclared，需要在 WSL2 Ubuntu 或 Linux 下跑。
 *
 * 实验方法（关键：必须开优化 -O2，-O0 不优化、看不出问题）：
 *   gcc -O2 demo3_volatile_flag.c -o demo3 && ./demo3
 *   不加 volatile：卡在 while(!flag)（-O2 死循环），需按 Ctrl+C 结束
 *   加 volatile  ：1 秒后打印 escaped, flag = 1
 *
 * 想看“罪证”的汇编：
 *   gcc -O2 -S demo3_volatile_flag.c -o demo3.s
 *   不加 volatile：循环体只剩 jmp 空转（只读了一次内存）
 *   加 volatile  ：循环里每圈 movl 重新读内存
 */
#include <stdio.h>
#include <signal.h>
#include <unistd.h>

int flag = 0;                  /* 反例：不加 volatile，-O2 下会被缓存进寄存器 */
/* volatile int flag = 0; */    /* 正例：要观察差别，把这一行打开、上一行注释掉 */

void on_alarm(int sig)         /* 模拟中断服务程序（ISR）：中断发生时由系统调用 */
{
    (void)sig;
    flag = 1;                  /* 中断里把标志置 1 */
}

int main(void)
{
    signal(SIGALRM, on_alarm); /* 注册：闹钟信号到了就调用 on_alarm */
    alarm(1);                  /* 1 秒后触发闹钟（模拟“中断到来”） */

    while (!flag) { }          /* 主循环：不加 volatile 时 -O2 下这里死循环 */

    printf("escaped, flag = %d\n", flag);
    return 0;
}
