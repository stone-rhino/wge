+++
title = "MULTIPART_INVALID_QUOTING"
weight = 53
+++

**Description:** Multipart invalid quoting

**Syntax:** `MULTIPART_INVALID_QUOTING`

**Implementation Status:** No

WGE can parse this directive, but the functionality is not implemented yet.

**Example:**

```apache
SecRule MULTIPART_INVALID_QUOTING "@rx value" "id:1001,deny,msg:'Test'"
```
