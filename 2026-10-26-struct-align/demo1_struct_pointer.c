/*
 * demo1（练习1）：结构体基础 + 结构体指针访问成员 + offsetof 看成员偏移
 *
 * 要点：
 *   ① 结构体把类型不同的变量打包成一条"记录"，在内存里连续存放
 *   ② 变量访问用 . ，指针访问用 ->（-> 就是 (*p).member 的简写）
 *   ③ offsetof 宏打印成员偏移，能直观看出编译器插的填充字节
 *
 * 编译：gcc -Wall demo1_struct_pointer.c -o demo1 && ./demo1
 */
#include <stdio.h>
#include <stdint.h>
#include <stddef.h>          /* offsetof 宏（成员偏移量）在这里 */

typedef struct {
    uint8_t  id;             /* 设备号 */
    uint16_t value;          /* 数值 */
    char     name[8];        /* 名字 */
} Device_t;                  /* _t 表示"这是一种类型" */

int main(void)
{
    Device_t dev = { .id = 1, .value = 250, .name = "motor" };
    Device_t *p = &dev;                    /* 结构体指针：指向整个结构体的首地址 */

    /* 两种访问方式：变量用 . ，指针用 -> */
    printf("dev.id    = %u, p->id    = %u\n", dev.id, p->id);
    printf("dev.value = %u, p->value = %u\n", dev.value, p->value);
    printf("p->name   = %s\n", p->name);

    p->value = 300;                        /* 通过指针改，原结构体跟着变（同一块内存） */
    printf("after p->value = 300, dev.value = %u\n", dev.value);

    /* offsetof：每个成员相对于结构体起始地址的偏移（字节） */
    printf("offsetof id    = %zu\n", offsetof(Device_t, id));
    printf("offsetof value = %zu\n", offsetof(Device_t, value));
    printf("offsetof name  = %zu\n", offsetof(Device_t, name));
    printf("sizeof(Device_t) = %zu\n", sizeof(Device_t));
    /* 注意：1+2+8 = 11，但 sizeof 是 12 —— 多出来的就是填充字节 */

    return 0;
}
