+++
title = "MULTIPART_BOUNDARY_QUOTED"
weight = 46
+++

**Description:** Multipart boundary quoting error

**Syntax:** `MULTIPART_BOUNDARY_QUOTED`

**Implementation Status:** No

**Example:**

```apache
SecRule MULTIPART_BOUNDARY_QUOTED "@rx value" "id:1001,deny,msg:'Test'"
```
