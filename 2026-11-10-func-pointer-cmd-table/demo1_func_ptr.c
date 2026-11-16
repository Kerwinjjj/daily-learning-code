/*
 * demo1：函数指针基础 —— 同一接口，传入不同计算函数
 *
 * 编译：gcc -Wall demo1_func_ptr.c -o demo1
 */
#include <stdio.h>

/* 三个"计算函数"：签名完全一致，都是 int (int, int) */
int add(int a, int b) { return a + b; }
int sub(int a, int b) { return a - b; }
int mul(int a, int b) { return a * b; }

/* 函数指针类型：指向"两个 int 进、一个 int 出"的函数 */
typedef int (*CalcFunc)(int, int);

/* 同一个接口，"怎么算"由传入的函数决定 */
int calc(int a, int b, CalcFunc fn)
{
    if (fn == NULL) {          /* 函数指针也可能是空指针，传错了就是跳到野地址 */
        return 0;
    }
    return fn(a, b);           /* 通过函数指针调用 */
}

int main(void)
{
    /* 最原始的写法：直接定义函数指针变量 */
    int (*p)(int, int) = add;

    printf("p(3, 4)    = %d\n", p(3, 4));       /* 两种调用写法等价 */
    printf("(*p)(3, 4) = %d\n", (*p)(3, 4));

    /* 函数名就是函数地址（不加括号、不加 & 也成立） */
    printf("&add = %p\n", (void *)&add);
    printf("add  = %p\n", (void *)add);

    /* 同一接口，三种行为 */
    printf("calc(10,5,add) = %d\n", calc(10, 5, add));
    printf("calc(10,5,sub) = %d\n", calc(10, 5, sub));
    printf("calc(10,5,mul) = %d\n", calc(10, 5, mul));

    return 0;
}
