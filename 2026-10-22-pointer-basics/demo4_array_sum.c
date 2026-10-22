/*
 * demo4（练习2）：用指针实现数组求和（踩坑重点：不要返回局部数组）
 *
 * 要点：
 *   1) 形参 int *arr 接收实参数组的首元素地址；n 传元素个数
 *   2) const 修饰形参：只读不修改，嵌入式写法习惯
 *   3) 函数返回 int 值（sum），不是地址——安全
 *   4) 绝不能 return 函数内部"栈上的局部数组"（详见 README 思考题）
 *
 * 编译：gcc demo4_array_sum.c -o demo4 && ./demo4
 */
#include <stdio.h>

int sum_array(const int *arr, int n)   /* 指针参数：数组首元素地址 + 长度 */
{
    int sum = 0;
    const int *p;

    for (p = arr; p < arr + n; p++) {
        sum += *p;                     /* 解引用逐个累加 */
    }
    return sum;                        /* 返回 int 值（不是地址），安全 */
}

int main(void)
{
    int data[5] = {1, 2, 3, 4, 5};
    printf("sum = %d\n", sum_array(data, 5));  /* 数组名退化为指针传入 */
    return 0;
}
