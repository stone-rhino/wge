+++
title = "t:uppercase"
weight = 3
+++

**Description:** Convert to uppercase

**Syntax:** `t:uppercase`

Converts all characters to uppercase using the current C locale settings.

**Example:**

```apache
SecRule ARGS "@rx TEST" "id:1001,t:uppercase"
```

**Parameter Type:** `string`

**Implementation Status:** Not yet implemented
