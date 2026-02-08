+++
title = "MULTIPART_DATA_BEFORE"
weight = 48
+++

**Description:** Data before multipart boundary

**Syntax:** `MULTIPART_DATA_BEFORE`

**Implementation Status:** No

**Example:**

```apache
SecRule MULTIPART_DATA_BEFORE "@rx value" "id:1001,deny,msg:'Test'"
```
