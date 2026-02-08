+++
title = "URLENCODED_ERROR"
weight = 63
+++

**Description:** URL encoding error

**Syntax:** `URLENCODED_ERROR`

**Implementation Status:** No

**Example:**

```apache
SecRule URLENCODED_ERROR "@rx value" "id:1001,deny,msg:'Test'"
```
