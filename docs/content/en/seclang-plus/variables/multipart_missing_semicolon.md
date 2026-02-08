+++
title = "MULTIPART_MISSING_SEMICOLON"
weight = 52
+++

**Description:** Multipart missing semicolon

**Syntax:** `MULTIPART_MISSING_SEMICOLON`

**Implementation Status:** No

**Example:**

```apache
SecRule MULTIPART_MISSING_SEMICOLON "@rx value" "id:1001,deny,msg:'Test'"
```
