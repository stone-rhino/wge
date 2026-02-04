+++
title = "URLENCODED_ERROR"
weight = 63
+++

**Description:** URL encoding error

**Syntax:** `URLENCODED_ERROR`

**Example:**

```apache
SecRule URLENCODED_ERROR "@rx value" "id:1001,deny,msg:'Test'"
```

**Implementation Status:** Not yet implemented
