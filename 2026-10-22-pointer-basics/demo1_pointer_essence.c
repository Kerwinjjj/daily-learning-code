/*
 * demo1：指针本质——指针是存放"地址"的变量；& 取地址、* 解引用
 *
 * 观察点：
 *   1) p 的值 == &a：指针存放的就是变量的地址
 *   2) *p 等价于 a：解引用后既能读也能写
 *   3) 指针 p 自己也有地址（&p），说明"指针也是变量"
 *
 * 编译：gcc demo1_pointer_essence.c -o demo1 && ./demo1
 */
#include <stdio.h>

int main(void)
{
    int a = 100;            /* 普通变量：在栈上，有自己的地址和值 */
    int *p = &a;            /* 指针变量：p 存放 a 的地址（& 取地址） */

    printf("a 的值    = %d\n", a);
    printf("a 的地址  = %p\n", (void *)&a);   /* & 取地址 */
    printf("p 的值    = %p\n", (void *)p);    /* p 存的就是 a 的地址 */
    printf("p 的地址  = %p\n", (void *)&p);   /* 指针也是变量，自己也有地址 */
    printf("*p        = %d\n", *p);           /* * 解引用：取出指向内存的值 */

    *p = 200;                /* 解引用赋值 == 直接给 a 赋值 */
    printf("修改后 a  = %d\n", a);            /* 输出 200 */

    return 0;
}
