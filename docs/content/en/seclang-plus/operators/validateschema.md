+++
title = "@validateSchema"
weight = 25
+++

**Description:** Validate XML against Schema

**Syntax:** `"@validateSchema parameter"`

**Example:**

```apache
SecRule ARGS "@validateSchema parameter" "id:1001,deny,msg:'Test'"
```

**Parameter Type:** `string`

**Implementation Status:** Not yet implemented
