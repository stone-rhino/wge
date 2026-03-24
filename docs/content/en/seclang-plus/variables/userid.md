+++
title = "USERID"
weight = 71
+++

**Description:** User ID

**Syntax:** `USERID`

This variable contains the value set with `setuid`.

**Implementation Status:** No

WGE can parse this directive, but the functionality is not implemented yet.

**Example:**

```apache
SecRule USERID "@rx value" "id:1001,deny,msg:'Test'"
```
