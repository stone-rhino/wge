+++
title = "t:jsDecode"
weight = 14
+++

**描述:** 对字符串进行 ``JavaScript`` 转义序列还原。

如果 ``\uHHHH`` 码点在 ``FF01-FF5E`` 范围内（全角 ASCII），则使用高字节来检测和调整低字节。否则，仅使用低字节，并将高字节置零（可能导致信息丢失）。

**语法:** `t:jsDecode`

**是否实现:** 是

**示例:**

```apache
SecRule ARGS "@rx test" "id:1001,t:jsDecode"
```
