+++
title = "@fuzzyHash"
weight = 34
+++

**Description:** Fuzzy hash matching

**Syntax:** `"@fuzzyHash parameter"`

**Example:**

```apache
SecRule ARGS "@fuzzyHash parameter" "id:1001,deny,msg:'Test'"
```

**Parameter Type:** `string`

**Implementation Status:** Not yet implemented
