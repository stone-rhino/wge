+++
title = "MULTIPART_NAME"
weight = 41
+++

**Description:** Multipart field name

**Syntax:** `MULTIPART_NAME`

**Implementation Status:** No

**Example:**

```apache
SecRule MULTIPART_NAME "@rx value" "id:1001,deny,msg:'Test'"
```
