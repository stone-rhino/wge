+++
title = "FILES_TMP_CONTENT"
weight = 79
+++

**Description:** Uploaded file temporary content

**Syntax:** `FILES_TMP_CONTENT`

**Example:**

```apache
SecRule FILES_TMP_CONTENT "@rx value" "id:1001,deny,msg:'Test'"
```

**Implementation Status:** Not yet implemented
