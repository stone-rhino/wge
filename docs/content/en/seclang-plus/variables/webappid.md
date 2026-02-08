+++
title = "WEBAPPID"
weight = 72
+++

**Description:** Web application ID

**Syntax:** `WEBAPPID`

**Implementation Status:** No

**Example:**

```apache
SecRule WEBAPPID "@rx value" "id:1001,deny,msg:'Test'"
```
