+++
title = "STATUS_LINE"
weight = 64
+++

**Description:** HTTP status line

**Syntax:** `STATUS_LINE`

**Implementation Status:** No

**Example:**

```apache
SecRule STATUS_LINE "@rx value" "id:1001,deny,msg:'Test'"
```
