+++
title = "MULTIPART_DATA_BEFORE"
weight = 48
+++

**Description:** Data before multipart boundary

**Syntax:** `MULTIPART_DATA_BEFORE`

**Example:**

```apache
SecRule MULTIPART_DATA_BEFORE "@rx value" "id:1001,deny,msg:'Test'"
```

**Implementation Status:** Not yet implemented
