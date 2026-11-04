/*
 * demo1（练习1）：const 三种写法 + 尝试修改观察报错
 *
 * 顺序记忆：const 离谁近，谁就不可改。
 *   const int *p        -> const 靠 *p：数据（*p）不可改，指针可以改指向
 *   int *const p        -> const 靠 p：指针（p）不可改，数据可以改
 *   const int *const p  -> 两边都靠：数据和指针都不能改
 *
 * 编译：gcc -Wall demo1_const_three_writes.c -o demo1 && ./demo1
 */
#include <stdio.h>

int main(void)
{
    int a = 1, b = 2;

    const int *p1 = &a;        /* 指向的数据是只读的（*p1 不能改） */
    int *const p2 = &a;        /* 指针本身是只读的（p2 的指向不能改） */
    const int *const p3 = &a;  /* 数据和指针都只读 */

    /* ---- 以下操作合法，能编译通过 ---- */
    p1 = &b;                   /* 改“指向”：合法（const 锁的是 *p1，不是 p1） */
    printf("p1 = &b -> *p1 = %d\n", *p1);   /* 2 */

    *p2 = 100;                 /* 改数据：合法（const 锁的是 p2，不是 *p2） */
    printf("a = %d\n", *p2);                /* 100 */

    printf("p3 = %d\n", *p3);               /* 100，p3 只能读 */

    /* ---- 以下三种操作非法，编译期报错（去掉注释即可复现，见 README 第 1 节） ----
     * *p1 = 10;   error: assignment of read-only location '*p1'
     * p2  = &b;   error: assignment of read-only variable 'p2'
     * *p3 = 10;   error: assignment of read-only location '*(const int *)p3'
     * p3  = &b;   error: assignment of read-only variable 'p3'
     */

    return 0;
}
