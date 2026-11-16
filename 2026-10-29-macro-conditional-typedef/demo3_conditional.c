/*
 * demo3：条件编译 —— 同一份代码适配不同硬件版本
 *   编译时用 -DHW_BOARD_A 或 -DHW_BOARD_B 指定板子
 *   不指定就 #error 直接拦住，避免"编出来但跑错板子"
 *
 * 编译：
 *   gcc -Wall -DHW_BOARD_A demo3_conditional.c -o demo3a
 *   gcc -Wall -DHW_BOARD_B demo3_conditional.c -o demo3b
 */
#include <stdio.h>

#ifdef HW_BOARD_A
  #define BOARD_NAME  "STM32F407"
  #define LED_PIN     0            /* PB0 */
  #define UART_BAUD   115200
#elif defined(HW_BOARD_B)
  #define BOARD_NAME  "ESP32"
  #define LED_PIN     2            /* GPIO2 */
  #define UART_BAUD   921600
#else
  #error "必须指定硬件版本：-DHW_BOARD_A 或 -DHW_BOARD_B"
#endif

int main(void)
{
    printf("board    = %s\n", BOARD_NAME);
    printf("led pin  = %d\n", LED_PIN);
    printf("baud     = %d\n", UART_BAUD);
    return 0;
}
