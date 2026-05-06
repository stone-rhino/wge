+++
title = "FILES_SIZES"
weight = 77
+++

**Description:** Uploaded file sizes

**Syntax:** `FILES_SIZES`

**Implementation Status:** No

WGE can parse this directive, but the functionality is not implemented yet.

**Example:**

```apache
SecRule FILES_SIZES "@rx value" "id:1001,deny,msg:'Test'"
```
