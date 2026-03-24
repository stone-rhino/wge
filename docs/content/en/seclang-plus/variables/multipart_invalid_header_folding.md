+++
title = "MULTIPART_INVALID_HEADER_FOLDING"
weight = 55
+++

**Description:** Multipart invalid header folding

**Syntax:** `MULTIPART_INVALID_HEADER_FOLDING`

**Implementation Status:** No

WGE can parse this directive, but the functionality is not implemented yet.

**Example:**

```apache
SecRule MULTIPART_INVALID_HEADER_FOLDING "@rx value" "id:1001,deny,msg:'Test'"
```
