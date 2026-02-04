+++
title = "MULTIPART_INVALID_HEADER_FOLDING"
weight = 55
+++

**Description:** Multipart invalid header folding

**Syntax:** `MULTIPART_INVALID_HEADER_FOLDING`

**Example:**

```apache
SecRule MULTIPART_INVALID_HEADER_FOLDING "@rx value" "id:1001,deny,msg:'Test'"
```

**Implementation Status:** Not yet implemented
