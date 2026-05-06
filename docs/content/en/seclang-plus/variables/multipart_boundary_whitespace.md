+++
title = "MULTIPART_BOUNDARY_WHITESPACE"
weight = 47
+++

**Description:** Multipart boundary whitespace error

**Syntax:** `MULTIPART_BOUNDARY_WHITESPACE`

**Implementation Status:** No

WGE can parse this directive, but the functionality is not implemented yet.

**Example:**

```apache
SecRule MULTIPART_BOUNDARY_WHITESPACE "@rx value" "id:1001,deny,msg:'Test'"
```
