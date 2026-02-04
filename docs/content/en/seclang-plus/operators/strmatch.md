+++
title = "@strmatch"
weight = 8
+++

**Description:** Wildcard matching

**Syntax:** `"@strmatch parameter"`

**Example:**

```apache
SecRule ARGS "@strmatch parameter" "id:1001,deny,msg:'Test'"
```

**Parameter Type:** `string (wildcard)`

**Implementation Status:** Not yet implemented
