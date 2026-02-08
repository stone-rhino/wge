+++
title = "SERVER_ADDR"
weight = 35
+++

**Description:** Server IP address

**Syntax:** `SERVER_ADDR`

**Implementation Status:** No

**Example:**

```apache
SecRule SERVER_ADDR "@rx value" "id:1001,deny,msg:'Test'"
```
