+++
title = "MULTIPART_FILE_LIMIT_EXCEEDED"
weight = 56
+++

**Description:** Multipart file count limit exceeded

**Syntax:** `MULTIPART_FILE_LIMIT_EXCEEDED`

**Example:**

```apache
SecRule MULTIPART_FILE_LIMIT_EXCEEDED "@rx value" "id:1001,deny,msg:'Test'"
```

**Implementation Status:** Not yet implemented
