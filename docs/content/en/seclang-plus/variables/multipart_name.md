+++
title = "MULTIPART_NAME"
weight = 41
+++

**Description:** Multipart field name

**Syntax:** `MULTIPART_NAME`

**Implementation Status:** No

WGE can parse this directive, but the functionality is not implemented yet.

**Example:**

```apache
SecRule MULTIPART_NAME "@rx value" "id:1001,deny,msg:'Test'"
```
