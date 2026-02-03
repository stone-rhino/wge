+++
title = "@rxGlobal"
weight = 12
+++

**Description:** Global regular expression matching (matches all occurrences)

**Syntax:** `"@rxGlobal parameter"`

**Example:**

```apache
SecRule ARGS "@rxGlobal ^admin" "id:1001,deny,msg:'Test'"
```

**Parameter Type:** `string (regex)`

**Implementation Status:** Not yet implemented
