/*
 * demo2：通过指针修改实参（传址 vs 传值）
 *
 * 为什么 C 函数改不了外面的变量？
 * 因为 C 默认传值（call by value）：形参是实参的拷贝，改拷贝不影响实参。
 * 传指针后，函数通过地址（& 取地址 + * 解引用）直接操作实参本身。
 *
 * 编译：gcc demo2_pointer_swap.c -o demo2 && ./demo2
 */
#include <stdio.h>

void swap_by_value(int x, int y)    /* 传值：只交换形参（拷贝），实参不变 */
{
    int t = x;
    x = y;
    y = t;
}

void swap_by_pointer(int *x, int *y) /* 传址：通过地址修改实参 */
{
    int t = *x;    /* 解引用：读出 x 指向的实参的值 */
    *x = *y;       /* 解引用：把 y 指向的值写入 x 指向的内存 */
    *y = t;
}

int main(void)
{
    int a = 1, b = 2;

    swap_by_value(a, b);            /* 失败：a、b 仍是 1 2 */
    printf("传值后：a = %d, b = %d\n", a, b);

    swap_by_pointer(&a, &b);        /* 成功：a、b 交换为 2 1 */
    printf("传址后：a = %d, b = %d\n", a, b);

    return 0;
}
