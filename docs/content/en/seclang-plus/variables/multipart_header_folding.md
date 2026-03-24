+++
title = "MULTIPART_HEADER_FOLDING"
weight = 50
+++

**Description:** Multipart header folding

**Syntax:** `MULTIPART_HEADER_FOLDING`

**Implementation Status:** No

WGE can parse this directive, but the functionality is not implemented yet.

**Example:**

```apache
SecRule MULTIPART_HEADER_FOLDING "@rx value" "id:1001,deny,msg:'Test'"
```
