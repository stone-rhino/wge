+++
title = "SESSIONID"
weight = 70
+++

**Description:** Session ID

**Syntax:** `SESSIONID`

**Example:**

```apache
SecRule SESSIONID "@rx value" "id:1001,deny,msg:'Test'"
```

**Implementation Status:** Not yet implemented
