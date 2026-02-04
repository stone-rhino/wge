+++
title = "t:urlDecodeUni"
weight = 5
+++

**描述:** Unicode URL 解码

**语法:** `t:urlDecodeUni`

**示例:**

```apache
SecRule ARGS "@rx test" "id:1001,t:urlDecodeUni"
```


**参数类型:** `string`


与 urlDecode 功能相同，但支持微软专有 %u 编码。若代码值在 FF01-FF5E 范围内（全宽 ASCII 码），则使用高位字节检测并调整低位字节；否则仅使用低位字节，高位字节清零。
