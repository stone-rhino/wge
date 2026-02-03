+++
title = "t:hexDecode"
weight = 11
+++

**描述:** 十六进制解码

**语法:** `t:hexDecode`

**示例:**

```apache
SecRule ARGS "@rx test" "id:1001,t:hexDecode"
```


**参数类型:** `string`


解码采用与 hexEncode 相同算法编码的字符串（参见后续条目）。
