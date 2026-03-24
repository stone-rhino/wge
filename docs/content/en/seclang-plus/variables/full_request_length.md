+++
title = "FULL_REQUEST_LENGTH"
weight = 23
+++

**Description:** Complete request length

**Syntax:** `FULL_REQUEST_LENGTH`

**Implementation Status:** No

WGE can parse this directive, but the functionality is not implemented yet.

**Example:**

```apache
SecRule FULL_REQUEST_LENGTH "@gt 10485760" "id:1001,deny,msg:'Test'"
```
