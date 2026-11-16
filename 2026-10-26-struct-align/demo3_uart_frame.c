/*
 * demo3（练习3）：自定义串口通信帧 —— 用 packed 保证字节数可控（小车协议铺垫）
 *
 * 帧格式约定：帧头(1 字节) + 角度(4 字节) + 校验(2 字节) = 7 字节
 *
 * 关键：结构体默认对齐会插入填充字节，把 7 字节的协议撑成 12 字节，
 *       对端按 7 字节解析就会错位。packed 强制紧排，字节数与协议一致。
 *
 * 编译：gcc -Wall demo3_uart_frame.c -o demo3 && ./demo3
 */
#include <stdio.h>
#include <stdint.h>
#include <string.h>

/* 串口帧（正确做法：packed） */
typedef struct {
    uint8_t  header;      /* 帧头，例如 0xAA */
    float    angle;       /* 数据：角度值 */
    uint16_t checksum;    /* 校验位 */
} __attribute__((packed)) UartFrame_t;

/* 对照组：不加 packed 的"普通结构体" */
typedef struct {
    uint8_t  header;
    float    angle;
    uint16_t checksum;
} NormalFrame_t;

/* 把结构体按字节打印出来，模拟串口线上真实传输的内容（小端字节序） */
static void dump_bytes(const void *p, size_t n)
{
    const unsigned char *b = (const unsigned char *)p;
    for (size_t i = 0; i < n; i++) {
        printf("%02X ", b[i]);
    }
    printf("\n");
}

int main(void)
{
    UartFrame_t frame;
    frame.header   = 0xAA;
    frame.angle    = 30.5f;          /* float 30.5 = 0x41F40000，小端存成 00 00 F4 41 */
    frame.checksum = 0x1234;

    printf("sizeof(UartFrame_t)  = %zu\n", sizeof(UartFrame_t));
    printf("packed 帧字节        : ");
    dump_bytes(&frame, sizeof(frame));

    NormalFrame_t normal;
    normal.header   = 0xAA;
    normal.angle    = 30.5f;
    normal.checksum = 0x1234;

    printf("sizeof(NormalFrame_t) = %zu\n", sizeof(NormalFrame_t));
    printf("普通帧字节（含填充）  : ");
    dump_bytes(&normal, sizeof(normal));

    /* 注意：普通版里 3 字节填充的内容是未定义的内存残留，
     * 这里恰好是 00 是因为栈上新分配、还没被写过 —— 千万不能依赖它是 0。 */

    return 0;
}
