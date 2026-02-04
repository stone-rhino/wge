+++
title = "@ipMatchFromFile"
weight = 20
+++

**Description:** Load IP address list from file for matching

**Syntax:** `"@ipMatchFromFile parameter"`

**Example:**

```apache
SecRule ARGS "@ipMatchFromFile 192.168.1.1" "id:1001,deny,msg:'Test'"
```

**Parameter Type:** `string (file path)`

**Implementation Status:** Not yet implemented
