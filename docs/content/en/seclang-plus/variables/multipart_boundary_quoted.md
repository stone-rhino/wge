+++
title = "MULTIPART_BOUNDARY_QUOTED"
weight = 46
+++

**Description:** Multipart boundary quoting error

**Syntax:** `MULTIPART_BOUNDARY_QUOTED`

**Implementation Status:** No

WGE can parse this directive, but the functionality is not implemented yet.

**Example:**

```apache
SecRule MULTIPART_BOUNDARY_QUOTED "@rx value" "id:1001,deny,msg:'Test'"
```
