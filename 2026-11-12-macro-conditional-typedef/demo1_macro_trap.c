/*
 * demo1：危险宏对比
 *   ① 缺括号：展开后运算优先级全乱
 *   ② 自增副作用：宏参数被求值两次（条件成立的那个分支）
 *   ③ inline 函数对照：只求值一次，且有类型检查
 *
 * 编译：gcc -Wall demo1_macro_trap.c -o demo1
 * 看展开：gcc -E demo1_macro_trap.c | grep "int m"
 */
#include <stdio.h>

/* 错误示范：整体不加括号 */
#define SQUARE_BAD(x)   x * x

/* 正确示范：每个参数加括号 + 整体加括号 */
#define SQUARE_OK(x)    ((x) * (x))

/* 求最大值的宏：参数在展开后出现两次 */
#define MAX_BAD(a, b)   ((a) > (b) ? (a) : (b))

/* inline 函数版本：参数只求值一次，还有类型检查 */
static inline int max_inline(int a, int b) { return a > b ? a : b; }

int main(void)
{
    /* ① 缺括号：SQUARE_BAD(1+2) 展开成 1 + 2 * 1 + 2 = 5 */
    printf("SQUARE_BAD(1+2) = %d   （正确值 9）\n", SQUARE_BAD(1 + 2));
    printf("SQUARE_OK (1+2) = %d   （正确值 9）\n", SQUARE_OK(1 + 2));

    /* ② 自增副作用：i=5 时 5>3 成立，取第一个分支 (i++)，i 被加两次 */
    int i = 5;
    int m = MAX_BAD(i++, 3);
    printf("MAX_BAD(i++, 3) = %d, 之后 i = %d   （i 应为 6）\n", m, i);

    /* ③ inline：实参只求值一次，j 只加一次 */
    int j = 5;
    int n = max_inline(j++, 3);
    printf("max_inline(j++,3) = %d, 之后 j = %d   （j 应为 6）\n", n, j);

    return 0;
}
