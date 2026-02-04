+++
title = "t:length"
weight = 33
+++

**描述:** 获取字符串长度

**语法:** `t:length`

**示例:**

```apache
SecRule ARGS "@rx test" "id:1001,t:length"
```


**参数类型:** `string`


查询输入字符串的长度（以字节为单位），并将结果（作为字符串）输出。例如输入 ABCDE 时，该转换函数将返回 5。
