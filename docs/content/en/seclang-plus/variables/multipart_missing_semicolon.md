+++
title = "MULTIPART_MISSING_SEMICOLON"
weight = 52
+++

**Description:** Multipart missing semicolon

**Syntax:** `MULTIPART_MISSING_SEMICOLON`

**Implementation Status:** No

WGE can parse this directive, but the functionality is not implemented yet.

**Example:**

```apache
SecRule MULTIPART_MISSING_SEMICOLON "@rx value" "id:1001,deny,msg:'Test'"
```
