+++
title = "URLENCODED_ERROR"
weight = 63
+++

**Description:** URL encoding error

**Syntax:** `URLENCODED_ERROR`

**Implementation Status:** No

WGE can parse this directive, but the functionality is not implemented yet.

**Example:**

```apache
SecRule URLENCODED_ERROR "@rx value" "id:1001,deny,msg:'Test'"
```
