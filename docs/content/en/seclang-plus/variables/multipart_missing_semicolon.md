+++
title = "MULTIPART_MISSING_SEMICOLON"
weight = 52
+++

**Description:** Multipart missing semicolon

**Syntax:** `MULTIPART_MISSING_SEMICOLON`

**Example:**

```apache
SecRule MULTIPART_MISSING_SEMICOLON "@rx value" "id:1001,deny,msg:'Test'"
```

**Implementation Status:** Not yet implemented
