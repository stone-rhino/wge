+++
title = "MSC_PCRE_LIMITS_EXCEEDED"
weight = 66
+++

**Description:** PCRE limits exceeded flag

**Syntax:** `MSC_PCRE_LIMITS_EXCEEDED`

**Implementation Status:** No

WGE can parse this directive, but the functionality is not implemented yet.

**Example:**

```apache
SecRule MSC_PCRE_LIMITS_EXCEEDED "@rx value" "id:1001,deny,msg:'Test'"
```
