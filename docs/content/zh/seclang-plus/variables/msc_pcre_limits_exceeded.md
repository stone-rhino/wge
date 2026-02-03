+++
title = "MSC_PCRE_LIMITS_EXCEEDED"
weight = 66
+++

**描述:** PCRE 限制超出标志


**语法:** `MSC_PCRE_LIMITS_EXCEEDED`


**示例:**


```apache
SecRule MSC_PCRE_LIMITS_EXCEEDED "@rx value" "id:1001,deny,msg:'Test'"
```


**是否实现:** 暂无
