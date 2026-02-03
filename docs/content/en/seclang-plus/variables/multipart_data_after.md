+++
title = "MULTIPART_DATA_AFTER"
weight = 49
+++

**Description:** Data after multipart boundary

**Syntax:** `MULTIPART_DATA_AFTER`

**Example:**

```apache
SecRule MULTIPART_DATA_AFTER "@rx value" "id:1001,deny,msg:'Test'"
```

**Implementation Status:** Not yet implemented
