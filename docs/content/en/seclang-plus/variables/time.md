+++
title = "TIME"
weight = 93
+++

**Description:** Current time (formatted string)

**Syntax:** `TIME`

**Example:**

```apache
SecRule TIME "@rx value" "id:1001,deny,msg:'Test'"
```

**Implementation Status:** Not yet implemented
