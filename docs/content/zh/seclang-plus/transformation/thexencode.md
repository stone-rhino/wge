+++
title = "t:hexEncode"
weight = 12
+++

**描述:** 十六进制编码

**语法:** `t:hexEncode`

**示例:**

```apache
SecRule ARGS "@rx test" "id:1001,t:hexEncode"
```


**参数类型:** `string`


将字符串（可能包含二进制字符）编码为每个输入字节对应两个十六进制字符。例如，xyz 将编码为 78797a。
