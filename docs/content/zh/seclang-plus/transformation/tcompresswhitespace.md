+++
title = "t:compressWhitespace"
weight = 20
+++

**描述:** 对字符串进行空格压缩，连续空格压缩为一个空格。

此转换函数通过以下方式处理输入字符串：
 - 将所有空白符(包括``0x20, \f, \t, \n, \r, \v, 0xa0``) 转换为空格
 - 将连续空格压缩为一个空格

**语法:** `t:compressWhitespace`

**是否实现:** 是

**示例:**

```apache
SecRule ARGS "@rx test" "id:1001,t:compressWhitespace"
```