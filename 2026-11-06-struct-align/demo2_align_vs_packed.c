/*
 * demo2（练习2）：默认对齐 vs 成员重排 vs packed 压缩 —— sizeof 与成员偏移对比
 *
 * 三个结构体成员完全相同（uint8_t / uint32_t / uint8_t / uint16_t），
 * 只是排列方式和是否 packed 不同，用 sizeof + offsetof 看出差别。
 *
 * 编译：gcc -Wall demo2_align_vs_packed.c -o demo2 && ./demo2
 */
#include <stdio.h>
#include <stdint.h>
#include <stddef.h>

/* ① 默认对齐：成员照原顺序排，编译器在成员之间插入填充字节 */
typedef struct {
    uint8_t  a;      /* 偏移 0 */
    uint32_t b;      /* 对齐 4 → 偏移 4（1~3 是填充） */
    uint8_t  c;      /* 偏移 8 */
    uint16_t d;      /* 对齐 2 → 偏移 10（9 是填充） */
} Plain_t;

/* ② 同样成员，重排顺序：填充消失（对齐规则没变，只是排列变合理） */
typedef struct {
    uint32_t b;      /* 偏移 0 */
    uint16_t d;      /* 偏移 4 */
    uint8_t  a;      /* 偏移 6 */
    uint8_t  c;      /* 偏移 7 */
} Reorder_t;

/* ③ packed（紧凑的）：__attribute__((packed)) 取消所有对齐填充 */
typedef struct {
    uint8_t  a;      /* 偏移 0 */
    uint32_t b;      /* 偏移 1（不再要求 4 的倍数） */
    uint8_t  c;      /* 偏移 5 */
    uint16_t d;      /* 偏移 6 */
} __attribute__((packed)) Packed_t;

#define SHOW(T) \
    printf("%-10s sizeof=%2zu | a=%zu b=%zu c=%zu d=%zu\n", #T, sizeof(T), \
           offsetof(T, a), offsetof(T, b), offsetof(T, c), offsetof(T, d))

int main(void)
{
    SHOW(Plain_t);       /* 默认对齐：12 */
    SHOW(Reorder_t);     /* 重排顺序：8 */
    SHOW(Packed_t);      /* packed  ：8，但布局与 Reorder_t 不同 */
    return 0;
}
