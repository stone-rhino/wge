+++
title = "FILES_SIZES"
weight = 77
+++

**Description:** Uploaded file sizes

**Syntax:** `FILES_SIZES`

**Example:**

```apache
SecRule FILES_SIZES "@rx value" "id:1001,deny,msg:'Test'"
```

**Implementation Status:** Not yet implemented
