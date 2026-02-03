+++
title = "t:escapeSeqDecode"
weight = 16
+++

**描述:** 转义序列解码

**语法:** `t:escapeSeqDecode`

**示例:**

```apache
SecRule ARGS "@rx test" "id:1001,t:escapeSeqDecode"
```


**参数类型:** `string`


解码 ANSI C 转义序列：\a, \b, \f, \n, \r, \t, \v, \\, \?, \', \", \xHH（十六进制），\0OOO（八进制）。无效编码将保留在输出中。
