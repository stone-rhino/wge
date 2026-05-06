+++
title = "t:sha1"
weight = 35
+++

**描述:** 对字符串计算 ``SHA1`` 哈希。

**语法:** `t:sha1`

**是否实现:** 是

**示例:**

```apache
SecRule ARGS "@rx test" "id:1001,t:sha1"
```
