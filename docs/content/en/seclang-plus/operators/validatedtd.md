+++
title = "@validateDtd"
weight = 24
+++

**Description:** Validate XML against DTD

**Syntax:** `"@validateDtd parameter"`

**Example:**

```apache
SecRule ARGS "@validateDtd parameter" "id:1001,deny,msg:'Test'"
```

**Parameter Type:** `string`

**Implementation Status:** Not yet implemented
