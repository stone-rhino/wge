+++
title = "@containsWord"
weight = 6
+++

**Description:** Contains the specified word

**Syntax:** `"@containsWord parameter"`

**Example:**

```apache
SecRule ARGS "@containsWord parameter" "id:1001,deny,msg:'Test'"
```

**Parameter Type:** `string`

**Implementation Status:** Not yet implemented
