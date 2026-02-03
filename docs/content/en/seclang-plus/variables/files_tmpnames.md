+++
title = "FILES_TMPNAMES"
weight = 78
+++

**Description:** Uploaded file temporary names

**Syntax:** `FILES_TMPNAMES`

**Example:**

```apache
SecRule FILES_TMPNAMES "@rx value" "id:1001,deny,msg:'Test'"
```

**Implementation Status:** Not yet implemented
