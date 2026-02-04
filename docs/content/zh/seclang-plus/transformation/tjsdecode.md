+++
title = "t:jsDecode"
weight = 14
+++

**描述:** JavaScript 解码

**语法:** `t:jsDecode`

**示例:**

```apache
SecRule ARGS "@rx test" "id:1001,t:jsDecode"
```


**参数类型:** `string`


解码 JavaScript 转义序列。若 \uHHHH 代码位于 FF01-FF5E 范围（全宽 ASCII 码），则使用高位字节检测并调整低位字节；否则仅使用低位字节，高位字节清零（可能导致信息丢失）。
