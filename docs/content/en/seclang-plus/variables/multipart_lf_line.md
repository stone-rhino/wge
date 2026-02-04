+++
title = "MULTIPART_LF_LINE"
weight = 51
+++

**Description:** Multipart LF line

**Syntax:** `MULTIPART_LF_LINE`

**Example:**

```apache
SecRule MULTIPART_LF_LINE "@rx value" "id:1001,deny,msg:'Test'"
```

**Implementation Status:** Not yet implemented
