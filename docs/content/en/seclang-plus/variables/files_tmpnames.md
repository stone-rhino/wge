+++
title = "FILES_TMPNAMES"
weight = 78
+++

**Description:** Uploaded file temporary names

**Syntax:** `FILES_TMPNAMES`

**Implementation Status:** No

WGE can parse this directive, but the functionality is not implemented yet.

**Example:**

```apache
SecRule FILES_TMPNAMES "@rx value" "id:1001,deny,msg:'Test'"
```
