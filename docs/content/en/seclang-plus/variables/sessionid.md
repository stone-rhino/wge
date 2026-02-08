+++
title = "SESSIONID"
weight = 70
+++

**Description:** Session ID

**Syntax:** `SESSIONID`

**Implementation Status:** No

**Example:**

```apache
SecRule SESSIONID "@rx value" "id:1001,deny,msg:'Test'"
```
