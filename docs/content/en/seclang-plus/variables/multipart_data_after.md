+++
title = "MULTIPART_DATA_AFTER"
weight = 49
+++

**Description:** Data after multipart boundary

**Syntax:** `MULTIPART_DATA_AFTER`

**Implementation Status:** No

**Example:**

```apache
SecRule MULTIPART_DATA_AFTER "@rx value" "id:1001,deny,msg:'Test'"
```
