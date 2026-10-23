/*
 * demo2（练习2）：数组指针 sizeof 对比——看懂解引用跨度差别
 *
 * 观察点（64 位平台）：
 *   sizeof(arr) = 20  整个数组
 *   sizeof(p)   = 8   指针本身（p 是数组指针，指向 int[5]）
 *   sizeof(*p)  = 20  *p 是"整个数组"（解引用跨度 = 数组宽度）
 *   sizeof(q)   = 8   指针本身（q 是普通指针，指向 int）
 *   sizeof(*q)  = 4   *q 是"一个 int"（解引用跨度 = 元素宽度）
 *
 * 编译：gcc demo2_array_pointer_sizeof.c -o demo2 && ./demo2
 */
#include <stdio.h>

int main(void)
{
    int arr[5] = {1, 2, 3, 4, 5};
    int (*p)[5] = &arr;    /* 数组指针：指向整个数组（必须 &arr，类型 int(*)[5]） */
    int *q = arr;          /* 普通指针：指向首元素（arr 退化为 int*） */

    printf("sizeof(arr) = %zu\n", sizeof(arr));   /* 20 */
    printf("sizeof(p)   = %zu\n", sizeof(p));     /* 8：指针本身 */
    printf("sizeof(*p)  = %zu\n", sizeof(*p));    /* 20：*p 是 int[5] */
    printf("sizeof(q)   = %zu\n", sizeof(q));     /* 8：指针本身 */
    printf("sizeof(*q)  = %zu\n", sizeof(*q));    /* 4：*q 是单个 int */
    printf("(*p)[2] = %d\n", (*p)[2]);            /* 3：先解引用拿数组，再下标 */

    return 0;
}
