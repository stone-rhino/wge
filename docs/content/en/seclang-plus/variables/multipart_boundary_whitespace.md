+++
title = "MULTIPART_BOUNDARY_WHITESPACE"
weight = 47
+++

**Description:** Multipart boundary whitespace error

**Syntax:** `MULTIPART_BOUNDARY_WHITESPACE`

**Implementation Status:** No

**Example:**

```apache
SecRule MULTIPART_BOUNDARY_WHITESPACE "@rx value" "id:1001,deny,msg:'Test'"
```
