+++
title = "MULTIPART_DATA_AFTER"
weight = 49
+++

**Description:** Data after multipart boundary

**Syntax:** `MULTIPART_DATA_AFTER`

**Implementation Status:** No

WGE can parse this directive, but the functionality is not implemented yet.

**Example:**

```apache
SecRule MULTIPART_DATA_AFTER "@rx value" "id:1001,deny,msg:'Test'"
```
