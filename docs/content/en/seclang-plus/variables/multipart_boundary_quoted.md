+++
title = "MULTIPART_BOUNDARY_QUOTED"
weight = 46
+++

**Description:** Multipart boundary quoting error

**Syntax:** `MULTIPART_BOUNDARY_QUOTED`

**Example:**

```apache
SecRule MULTIPART_BOUNDARY_QUOTED "@rx value" "id:1001,deny,msg:'Test'"
```

**Implementation Status:** Not yet implemented
