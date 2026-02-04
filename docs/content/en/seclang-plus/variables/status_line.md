+++
title = "STATUS_LINE"
weight = 64
+++

**Description:** HTTP status line

**Syntax:** `STATUS_LINE`

**Example:**

```apache
SecRule STATUS_LINE "@rx value" "id:1001,deny,msg:'Test'"
```

**Implementation Status:** Not yet implemented
