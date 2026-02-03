+++
title = "MSC_PCRE_LIMITS_EXCEEDED"
weight = 66
+++

**Description:** PCRE limits exceeded flag

**Syntax:** `MSC_PCRE_LIMITS_EXCEEDED`

**Example:**

```apache
SecRule MSC_PCRE_LIMITS_EXCEEDED "@rx value" "id:1001,deny,msg:'Test'"
```

**Implementation Status:** Not yet implemented
