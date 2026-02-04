+++
title = "t:replaceNulls"
weight = 24
+++

**描述:** 替换空字符

**语法:** `t:replaceNulls`

**示例:**

```apache
SecRule ARGS "@rx test" "id:1001,t:replaceNulls"
```


**参数类型:** `string`


将输入中的 NUL 字节替换为空格字符（ASCII 0x20）。
