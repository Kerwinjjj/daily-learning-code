# Daily Learning Code 日常学习代码

个人日常学习代码仓库：以 C 语言 / 嵌入式方向为主，记录每天的学习笔记和练习 demo。

## 目录结构

| 日期 | 主题 | 内容 |
|------|------|------|
| 2026-10-21 | 程序内存四区 & static 关键字 | 内存分区地址打印（demo1）、static 局部静态变量（demo2）、学习笔记与面试题解答 |
| 2026-10-22 | C 指针基础 & 指针与数组 | 指针本质/数组名 vs 指针/指针运算（demo1、demo2）、遍历数组两种写法（demo3）、指针数组求和（demo4）、思考题解答 |
| 2026-10-23 | C 指针进阶：二级指针/指针数组/数组指针 | 声明读法（demo1 命令表）、数组指针 sizeof 对比（demo2）、口述题解答 |

## 说明

- 每个主题一个目录：`YYYY-MM-DD-主题/`
- 每个目录内包含：demo 源码 + README 学习笔记
- 编译运行示例（gcc）：

```bash
gcc demo1_memory_addresses.c -o demo1 && ./demo1
gcc demo2_static_local_var.c -o demo2 && ./demo2
```
