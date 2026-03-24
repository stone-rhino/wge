+++
title = "MULTIPART_DATA_BEFORE"
weight = 48
+++

**Description:** Data before multipart boundary

**Syntax:** `MULTIPART_DATA_BEFORE`

**Implementation Status:** No

WGE can parse this directive, but the functionality is not implemented yet.

**Example:**

```apache
SecRule MULTIPART_DATA_BEFORE "@rx value" "id:1001,deny,msg:'Test'"
```
