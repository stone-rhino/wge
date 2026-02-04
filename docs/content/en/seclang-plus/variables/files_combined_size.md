+++
title = "FILES_COMBINED_SIZE"
weight = 80
+++

**Description:** Combined size of all uploaded files

**Syntax:** `FILES_COMBINED_SIZE`

**Example:**

```apache
SecRule FILES_COMBINED_SIZE "@rx value" "id:1001,deny,msg:'Test'"
```

**Implementation Status:** Not yet implemented
