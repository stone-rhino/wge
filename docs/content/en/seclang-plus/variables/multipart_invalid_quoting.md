+++
title = "MULTIPART_INVALID_QUOTING"
weight = 53
+++

**Description:** Multipart invalid quoting

**Syntax:** `MULTIPART_INVALID_QUOTING`

**Example:**

```apache
SecRule MULTIPART_INVALID_QUOTING "@rx value" "id:1001,deny,msg:'Test'"
```

**Implementation Status:** Not yet implemented
