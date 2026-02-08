+++
title = "MULTIPART_FILENAME"
weight = 40
+++

**Description:** Multipart filename

**Syntax:** `MULTIPART_FILENAME`

**Implementation Status:** No

**Example:**

```apache
SecRule MULTIPART_FILENAME "@rx value" "id:1001,deny,msg:'Test'"
```
