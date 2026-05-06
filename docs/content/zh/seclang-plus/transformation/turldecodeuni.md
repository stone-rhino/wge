+++
title = "t:urlDecodeUni"
weight = 5
+++

**描述:** 对字符串进行 ``URL`` 解码。

此转换函数与 ``t:urlDecode`` 类似，但支持 ``Microsoft`` 特定的 ``%u`` 编码。如果码点在 ``FF01-FF5E`` 范围内（全角 ``ASCII`` 码），则使用高字节来检测和调整低字节。否则，仅使用低字节，并将高字节置零。

**语法:** `t:urlDecodeUni`

**是否实现:** 是

**示例:**

```apache
SecRule ARGS "@rx test" "id:1001,t:urlDecodeUni"
```
