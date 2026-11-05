/*
 * demo2（练习2）：故意写错的版本 —— free 后不置 NULL，继续解引用
 *
 * 这段代码是用来"观察现象"的反面教材，不要照抄进工程。
 *
 * 错误清单：
 *   错误1：malloc 之后不判断返回值，直接 p[0] = 111
 *          → 分配失败时 p 是 NULL，NULL 解引用直接崩溃（单片机里是 HardFault 死机）
 *   错误2：free(p) 之后不置 NULL，继续读 p[0] p[1] p[2]
 *          → 野指针（指向已释放内存），属于 UB（未定义行为，Undefined Behavior）
 *   错误3：往已释放的内存里写 p[0] = 999
 *          → 一旦这块内存被别处 malloc 复用，就破坏了别人的数据
 *
 * 实测现象（重点）：程序不报错、不崩溃，读出来的还是 111 / 222 / 333，写 999 也"成功"。
 *   因为 free 只是把这块标记成"可再分配"，不一定立刻还给系统，内容还在原处。
 *   最危险的地方就在这里：看起来一切正常，实际已经埋雷 —— 等别处用了这块内存就会随机崩。
 *
 * 编译：gcc -Wall demo2_use_after_free.c -o demo2 && ./demo2
 */
#include <stdio.h>
#include <stdlib.h>

int main(void)
{
    int *p = (int *)malloc(sizeof(int) * 3);

    /* 错误1：没有判断 p 是否为 NULL 就直接用 */
    p[0] = 111;
    p[1] = 222;
    p[2] = 333;
    printf("before free: p[0]=%d p[1]=%d p[2]=%d\n", p[0], p[1], p[2]);

    free(p);
    /* 错误2：free 之后没有 p = NULL; —— 本该在这里补上 */

    /* 错误3：继续解引用已经释放的内存（野指针） */
    printf("after free : p[0]=%d p[1]=%d p[2]=%d\n", p[0], p[1], p[2]);

    /* 错误4：往已释放的内存里写，破坏的可能是别人的数据 */
    p[0] = 999;
    printf("after write: p[0]=%d\n", p[0]);

    /* 正确写法应该是：
     *   free(p);
     *   p = NULL;
     * 之后任何 p[0] 的访问都会立刻因空指针暴露问题，而不是潜伏下去。
     */

    return 0;
}
