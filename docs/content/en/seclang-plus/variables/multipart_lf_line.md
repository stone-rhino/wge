+++
title = "MULTIPART_LF_LINE"
weight = 51
+++

**Description:** Multipart LF line

**Syntax:** `MULTIPART_LF_LINE`

**Implementation Status:** No

WGE can parse this directive, but the functionality is not implemented yet.

**Example:**

```apache
SecRule MULTIPART_LF_LINE "@rx value" "id:1001,deny,msg:'Test'"
```
