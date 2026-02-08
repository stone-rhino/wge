+++
title = "MULTIPART_CRLF_LF_LINES"
weight = 43
+++

**Description:** Multipart CRLF/LF line count

**Syntax:** `MULTIPART_CRLF_LF_LINES`

**Implementation Status:** No

**Example:**

```apache
SecRule MULTIPART_CRLF_LF_LINES "@rx value" "id:1001,deny,msg:'Test'"
```
