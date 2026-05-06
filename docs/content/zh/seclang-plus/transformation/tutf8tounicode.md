+++
title = "t:utf8ToUnicode"
weight = 32
+++

**描述:** 对字符串的 ``UTF-8`` 字符序列进行 ``Unicode`` 编码。

此转换函数会对输入字符串所有 ``UTF-8`` 字符序列转换为 ``Unicode``（采用 ``%uHHHH`` 格式）。

**语法:** `t:utf8ToUnicode`

**是否实现:** 是

**示例:**

```apache
SecRule ARGS "@rx test" "id:1001,t:utf8ToUnicode"
```

