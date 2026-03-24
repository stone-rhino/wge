+++
title = "FILES_TMP_CONTENT"
weight = 79
+++

**Description:** Uploaded file temporary content

**Syntax:** `FILES_TMP_CONTENT | FILES_TMP_CONTENT:Key`

**Implementation Status:** No

WGE can parse this directive, but the functionality is not implemented yet.

**Example:**

```apache
SecRule FILES_TMP_CONTENT "@rx value" "id:1001,deny,msg:'Test'"
```
