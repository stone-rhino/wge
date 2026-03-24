+++
title = "STATUS_LINE"
weight = 64
+++

**Description:** HTTP status line

**Syntax:** `STATUS_LINE`

**Implementation Status:** No

WGE can parse this directive, but the functionality is not implemented yet.

**Example:**

```apache
SecRule STATUS_LINE "@rx value" "id:1001,deny,msg:'Test'"
```
