+++
title = "@inspectFile"
weight = 33
+++

**Description:** Invoke external script to inspect file

**Syntax:** `"@inspectFile parameter"`

**Example:**

```apache
SecRule ARGS "@inspectFile parameter" "id:1001,deny,msg:'Test'"
```

**Parameter Type:** `string (file path)`

**Implementation Status:** Not yet implemented
