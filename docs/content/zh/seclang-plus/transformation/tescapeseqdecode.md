+++
title = "t:escapeSeqDecode"
weight = 16
+++

**描述:** 对字符串中的 ``ANSI C`` 风格转义序列还原成真实字符。

此转换函数会还原 ``ANSI C`` 转义序列：\a、\b、\f、\n、\r、\t、\v、\\、\?、\'、\"、\xHH（十六进制）、\0OOO（八进制）。无效编码将保留在转换结果中。

**语法:** `t:escapeSeqDecode`

**是否实现:** 是

**示例:**

```apache
SecRule ARGS "@rx test" "id:1001,t:escapeSeqDecode"
```
