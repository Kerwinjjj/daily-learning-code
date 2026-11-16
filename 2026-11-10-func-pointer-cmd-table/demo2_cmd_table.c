/*
 * demo2：函数指针数组（命令表）—— 串口收到不同指令，调用不同处理函数
 *
 * 编译：gcc -Wall demo2_cmd_table.c -o demo2
 */
#include <stdio.h>
#include <string.h>

/* ---- 处理函数：统一签名 void (*)(void) ---- */
static void cmd_led_on(void)    { printf("[执行] 开灯\n"); }
static void cmd_led_off(void)   { printf("[执行] 关灯\n"); }
static void cmd_motor_run(void) { printf("[执行] 电机启动\n"); }
static void cmd_motor_stop(void){ printf("[执行] 电机停止\n"); }
static void cmd_status(void)    { printf("[执行] 打印状态\n"); }

typedef void (*CmdHandler)(void);       /* 处理函数的类型 */

/* 命令表的一项：指令字符串 + 处理函数 */
typedef struct {
    const char *name;
    CmdHandler  handler;
} CmdEntry;

/* 命令表：加一条指令只要加一行，不用改解析逻辑 */
static const CmdEntry cmd_table[] = {
    { "led on",     cmd_led_on },
    { "led off",    cmd_led_off },
    { "motor run",  cmd_motor_run },
    { "motor stop", cmd_motor_stop },
    { "status",     cmd_status },
};

#define CMD_COUNT (sizeof(cmd_table) / sizeof(cmd_table[0]))

/* 命令解析：遍历表，找到就调用对应函数 */
static int dispatch(const char *cmd)
{
    for (size_t i = 0; i < CMD_COUNT; i++) {
        if (strcmp(cmd, cmd_table[i].name) == 0) {
            cmd_table[i].handler();        /* 查表 → 调用 */
            return 0;
        }
    }
    printf("[未知指令] %s\n", cmd);
    return -1;
}

int main(void)
{
    /* 模拟串口收到的指令序列 */
    const char *rx_cmds[] = { "led on", "motor run", "status", "led off", "foo" };

    for (size_t i = 0; i < sizeof(rx_cmds) / sizeof(rx_cmds[0]); i++) {
        printf("收到: %s -> ", rx_cmds[i]);
        dispatch(rx_cmds[i]);
    }

    /* 最简版：纯函数指针数组（不带字符串匹配），按下标调用 */
    CmdHandler simple_table[] = { cmd_led_on, cmd_led_off };
    printf("--- 纯数组下标调用 ---\n");
    simple_table[0]();                 /* 相当于"0 号命令 = 开灯" */
    simple_table[1]();

    return 0;
}
