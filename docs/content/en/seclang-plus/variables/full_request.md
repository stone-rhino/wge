+++
title = "FULL_REQUEST"
weight = 22
+++

**Description:** Complete request data

**Syntax:** `FULL_REQUEST`

**Implementation Status:** No

WGE can parse this directive, but the functionality is not implemented yet.

**Example:**

```apache
SecRule FULL_REQUEST "@rx value" "id:1001,deny,msg:'Test'"
```
