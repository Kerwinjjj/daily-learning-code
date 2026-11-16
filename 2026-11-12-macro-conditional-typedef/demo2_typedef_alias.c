/*
 * demo2：typedef 给结构体、函数指针起别名
 *        顺带对比 #define 当类型别名会出什么事
 *
 * 编译：gcc -Wall demo2_typedef_alias.c -o demo2
 */
#include <stdio.h>

/* ① 结构体别名 */
typedef struct {
    int x;
    int y;
} Point_t;

/* ② 错误示范：#define 当类型别名（纯文本替换） */
#define PTR_INT int*

/* ③ 正确示范：typedef 当类型别名（编译器认的"类型"） */
typedef int *IntPtr;

/* ④ 函数指针别名（复用 Day8 的写法） */
typedef int (*CalcFunc)(int, int);

int add(int a, int b) { return a + b; }
int sub(int a, int b) { return a - b; }

int main(void)
{
    Point_t p = { 3, 4 };                /* 结构体别名，省掉 struct 关键字 */
    printf("p = (%d, %d)\n", p.x, p.y);

    /* #define 的坑：PTR_INT a, b; 展开成 int* a, b;
       结果 a 是指针，b 只是 int —— 两个变量类型不一样 */
    PTR_INT a, b;
    printf("PTR_INT  : sizeof(a)=%zu, sizeof(b)=%zu\n", sizeof(a), sizeof(b));

    /* typedef 没有这个问题：c、d 都是指针 */
    IntPtr c, d;
    printf("IntPtr   : sizeof(c)=%zu, sizeof(d)=%zu\n", sizeof(c), sizeof(d));

    /* 函数指针别名建表：同一个数组装两种运算 */
    CalcFunc ops[] = { add, sub };
    printf("ops[0](7,3) = %d\n", ops[0](7, 3));
    printf("ops[1](7,3) = %d\n", ops[1](7, 3));

    return 0;
}
