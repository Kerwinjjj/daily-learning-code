# Daily Learning Code 日常学习代码

个人日常学习代码仓库：以 C 语言 / 嵌入式方向为主，记录每天的学习笔记和练习 demo。

## 目录结构

| 日期 | 主题 | 内容 |
|------|------|------|
| 2026-10-21 | 程序内存四区 & static 关键字 | 内存分区地址打印（demo1）、static 局部静态变量（demo2）、学习笔记与面试题解答 |

## 说明

- 每个主题一个目录：`YYYY-MM-DD-主题/`
- 每个目录内包含：demo 源码 + README 学习笔记
- 编译运行示例（gcc）：

```bash
gcc demo1_memory_addresses.c -o demo1 && ./demo1
gcc demo2_static_local_var.c -o demo2 && ./demo2
```
