/*
 * demo3（练习1）：遍历数组的两种写法——下标 [] 方式、指针自增方式
 *
 * 要点：
 *   1) arr[i] 等价于 *(arr + i)，下标本质是"指针 + 偏移"
 *   2) 指针自增 p++ 移动的是"一个元素步长"（int 为 4 字节），不是 +1
 *   3) 循环条件 p < arr + n：用地址比较判断是否越界
 *
 * 编译：gcc demo3_array_traverse.c -o demo3 && ./demo3
 */
#include <stdio.h>

int main(void)
{
    int arr[5] = {10, 20, 30, 40, 50};
    int i;
    int *p;

    /* 写法一：下标 [] 方式 */
    printf("下标遍历：");
    for (i = 0; i < 5; i++) {
        printf("%d ", arr[i]);   /* arr[i] 等价于 *(arr + i) */
    }
    printf("\n");

    /* 写法二：指针自增方式 */
    printf("指针遍历：");
    for (p = arr; p < arr + 5; p++) {   /* p++：地址 + 4 字节（int 步长） */
        printf("%d ", *p);              /* 解引用当前指向的元素 */
    }
    printf("\n");

    return 0;
}
