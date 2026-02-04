+++
title = "@rbl"
weight = 36
+++

**Description:** Real-time blacklist lookup

**Syntax:** `"@rbl parameter"`

**Example:**

```apache
SecRule ARGS "@rbl parameter" "id:1001,deny,msg:'Test'"
```

**Parameter Type:** `string`

**Implementation Status:** Not yet implemented
