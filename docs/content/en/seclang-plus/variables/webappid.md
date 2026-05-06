+++
title = "WEBAPPID"
weight = 72
+++

**Description:** Web application ID

**Syntax:** `WEBAPPID`

**Implementation Status:** No

WGE can parse this directive, but the functionality is not implemented yet.

**Example:**

```apache
SecRule WEBAPPID "@rx value" "id:1001,deny,msg:'Test'"
```
