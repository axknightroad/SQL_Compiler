Linux SQL分析器 v0.5
================

### 简介
XDU本科编译原理大作业，用C语言实现的简单的 **Linux SQL** 分析器。

### 功能
用SQL语句进行数据库的创建与删除，表的创建与删除，数据行的添加、删除与更新，单表查询以及数据库的本地存储功能。

### 工具
* 词法分析工具 `flex`（前身为lex）
* 语法分析工具 `bison` （前身为yacc）
* C语言编译器：`GCC`

### 源文件说明：
* `sql.lex.l`：SQL词法分析器
* `sql.parsing.y`：SQL语法分析器
* `sql.struct.h`：程序中使用到的数据结构声明和定义以及使用到的函数的声明
* `sql.function.c`：主程序C例程以及函数定义
* `makefile`：SQL分析器的makefile文件
* `README.md`：说明文件

==================
