+++
title = "t:htmlEntityDecode"
weight = 13
+++

**描述:** 对字符串进行 ``HTML实体`` 转义序列还原。

**语法:** `t:htmlEntityDecode`

**是否实现:** 是

**示例:**

```apache
SecRule ARGS "@rx test" "id:1001,t:htmlEntityDecode"
```

