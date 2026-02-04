+++
title = "MULTIPART_FILENAME"
weight = 40
+++

**Description:** Multipart filename

**Syntax:** `MULTIPART_FILENAME`

**Example:**

```apache
SecRule MULTIPART_FILENAME "@rx value" "id:1001,deny,msg:'Test'"
```

**Implementation Status:** Not yet implemented
