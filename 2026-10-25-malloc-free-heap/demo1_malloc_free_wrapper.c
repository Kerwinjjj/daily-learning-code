/*
 * demo1（练习1）：封装"动态 int 数组"的创建与释放 —— 正确做法
 *
 * 三个要点：
 *   ① malloc 返回 void*（无类型指针），要强转成 int*
 *   ② 分配后立刻判断返回值是否为 NULL（坑1：不判断就解引用 = 崩溃）
 *   ③ free 之后立刻把指针置 NULL（坑2：不置空就成了野指针）
 *
 * 封装注意：释放函数必须收 int**（二级指针），
 *           因为 C 是按值传递，传 int* 进去只能改副本，改不到调用者的指针。
 *
 * 编译：gcc -Wall demo1_malloc_free_wrapper.c -o demo1 && ./demo1
 */
#include <stdio.h>
#include <stdlib.h>

#define N 5

/* 动态创建 int 数组：分配前判断 NULL */
int *create_int_array(size_t n)
{
    int *p = (int *)malloc(n * sizeof(int));   /* 用 sizeof 算大小，别写死 4 或 20 */
    if (p == NULL) {                           /* 坑1：分配失败必须判断 */
        printf("malloc failed (request %zu bytes)\n", n * sizeof(int));
        return NULL;
    }
    for (size_t i = 0; i < n; i++) {
        p[i] = (int)(i * 10);                  /* 这段内存现在归你，随便读写 */
    }
    return p;
}

/* 释放数组：free 之后把调用者的指针置 NULL */
void free_int_array(int **pp)                  /* 收二级指针，才改得到调用者手里的指针 */
{
    if (pp == NULL || *pp == NULL) {           /* 防重复释放：已经是 NULL 就什么都不做 */
        return;
    }
    free(*pp);                                 /* 归还给堆管理器 */
    *pp = NULL;                                /* 坑2：置 NULL，防止野指针 */
}

int main(void)
{
    int *arr = create_int_array(N);
    if (arr == NULL) {                         /* 坑1 的正确处理：失败就收工，不往下用 */
        return 1;
    }

    for (int i = 0; i < N; i++) {
        printf("arr[%d] = %d\n", i, arr[i]);
    }

    free_int_array(&arr);
    printf("after free: arr = %p\n", (void *)arr);   /* 已置空 → 打印全 0 */

    if (arr != NULL) {
        printf("BUG: 没有置空\n");
    } else {
        printf("ok: 指针已置空，后续误用会立刻暴露\n");
    }

    /* 实证坑1：申请超大内存时 malloc 真的会返回 NULL */
    int *big = create_int_array((size_t)1 << 60);    /* 约 4 EB，必然失败 */
    if (big == NULL) {
        printf("big array = NULL：分配失败，但程序没崩（因为判断了）\n");
    } else {
        free_int_array(&big);
    }

    return 0;
}
