+++
title = "SESSIONID"
weight = 70
+++

**Description:** Session ID

**Syntax:** `SESSIONID`

**Implementation Status:** No

WGE can parse this directive, but the functionality is not implemented yet.

**Example:**

```apache
SecRule SESSIONID "@rx value" "id:1001,deny,msg:'Test'"
```
