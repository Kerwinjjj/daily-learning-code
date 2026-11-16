/*
 * test.h —— 模块的"对外接口清单"
 *
 * 头文件里只放三类东西：
 *   1) 宏定义        (#define)
 *   2) 类型定义      (typedef / struct)
 *   3) 声明          (函数原型、extern 变量声明)
 *
 * 绝对不能放：函数体、变量定义（那是 .c 的事）
 */
#ifndef TEST_H          /* 头文件保护宏：第一次进来时 TEST_H 还没定义 */
#define TEST_H          /* 立刻定义它，第二次再进来就能被挡住 */

#include <stdint.h>

#define MAX_SAMPLES   8         /* 最多保存几个采样 */
#define MODULE_NAME   "sample"  /* 模块名，给打印用 */

typedef struct {
    uint8_t  id;                /* 采样编号 */
    uint16_t value;             /* 采样值 */
} Sample_t;                     /* _t 表示"这是一种类型" */

/* ---- 函数声明：只写原型，末尾是分号，没有函数体 ---- */
void     sample_init(void);
int      sample_add(uint16_t v);
uint16_t sample_avg(void);
int      sample_count(void);

/* ---- 全局变量：这里只能"声明"，关键字是 extern ---- */
extern int g_sample_total;      /* 告诉编译器：这个变量在别的 .c 里定义好了 */

#endif /* TEST_H */
