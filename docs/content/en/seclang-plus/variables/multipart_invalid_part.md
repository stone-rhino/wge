+++
title = "MULTIPART_INVALID_PART"
weight = 54
+++

**Description:** Multipart invalid part

**Syntax:** `MULTIPART_INVALID_PART`

**Implementation Status:** No

WGE can parse this directive, but the functionality is not implemented yet.

**Example:**

```apache
SecRule MULTIPART_INVALID_PART "@rx value" "id:1001,deny,msg:'Test'"
```
