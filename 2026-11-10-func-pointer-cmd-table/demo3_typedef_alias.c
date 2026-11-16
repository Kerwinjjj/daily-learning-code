/*
 * demo3：typedef 到底在做什么 —— 三种写法其实是一件事
 *
 * ① 直接声明函数指针变量       int (*raw)(int, int);
 * ② 先起类型名，再声明变量      typedef int (*CalcFunc)(int, int);  CalcFunc ali;
 * ③ 参数位置写完整类型 + 参数名 int calc(int a, int b, int (*fn)(int, int))
 *
 * 编译：gcc -Wall demo3_typedef_alias.c -o demo3
 */
#include <stdio.h>

int add(int a, int b) { return a + b; }

/* ① 不起名字：直接一个函数指针变量 */
int (*raw)(int, int) = add;

/* ② 先给"这类函数指针"起个类型名 */
typedef int (*CalcFunc)(int, int);

/* ③ 参数处写完整类型：fn 就是第三个参数的名字 */
static int calc_raw(int a, int b, int (*fn)(int, int))
{
    if (fn == NULL) { return 0; }
    return fn(a, b);
}

/* ③' 参数处用类型名：CalcFunc 是类型，fn 依然是名字 */
static int calc_alias(int a, int b, CalcFunc fn)
{
    if (fn == NULL) { return 0; }
    return fn(a, b);
}

int main(void)
{
    CalcFunc ali = add;                /* 用类型名声明变量 */

    /* 三种写法的大小完全一样：都是指针（64 位下 8 字节） */
    printf("sizeof(raw) = %zu, sizeof(ali) = %zu, sizeof(CalcFunc) = %zu\n",
           sizeof(raw), sizeof(ali), sizeof(CalcFunc));

    printf("raw(3,4) = %d, ali(3,4) = %d\n", raw(3, 4), ali(3, 4));
    printf("calc_raw(10,5,add)   = %d\n", calc_raw(10, 5, add));
    printf("calc_alias(10,5,add) = %d\n", calc_alias(10, 5, add));

    /* 用类型名的最大好处：函数指针数组一眼能读 */
    CalcFunc table[2] = { add, add };
    printf("table[0](5,6) = %d\n", table[0](5, 6));

    return 0;
}
