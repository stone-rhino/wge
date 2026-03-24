+++
title = "MSC_PCRE_LIMITS_EXCEEDED"
weight = 66
+++

**描述:** PCRE 限制超出标志

**语法:** `MSC_PCRE_LIMITS_EXCEEDED`

**是否实现:** 否

此指令WGE可以正常解析，但是暂未实现该功能。

**示例:**

```apache
SecRule MSC_PCRE_LIMITS_EXCEEDED "@rx value" "id:1001,deny,msg:'Test'"
```
