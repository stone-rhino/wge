+++
title = "t:compressWhitespace"
weight = 20
+++

**描述:** 压缩空白字符

**语法:** `t:compressWhitespace`

**示例:**

```apache
SecRule ARGS "@rx test" "id:1001,t:compressWhitespace"
```


**参数类型:** `string`


将任意空白字符（0x20, \f, \t, \n, \r, \v, 0xa0）转换为空格（ASCII 0x20），并将连续多个空格压缩为单个空格。
