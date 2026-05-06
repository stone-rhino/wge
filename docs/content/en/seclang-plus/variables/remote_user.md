+++
title = "REMOTE_USER"
weight = 34
+++

**Description:** Authenticated username

**Syntax:** `REMOTE_USER`

**Implementation Status:** No

WGE can parse this directive, but the functionality is not implemented yet.

**Example:**

```apache
SecRule REMOTE_USER "@rx value" "id:1001,deny,msg:'Test'"
```
