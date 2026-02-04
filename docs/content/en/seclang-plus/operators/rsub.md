+++
title = "@rsub"
weight = 37
+++

**Description:** Regular expression substitution

**Syntax:** `"@rsub parameter"`

**Example:**

```apache
SecRule ARGS "@rsub parameter" "id:1001,deny,msg:'Test'"
```

**Parameter Type:** `string`

**Implementation Status:** Not yet implemented
