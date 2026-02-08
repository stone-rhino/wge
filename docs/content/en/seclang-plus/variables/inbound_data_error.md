+++
title = "INBOUND_DATA_ERROR"
weight = 61
+++

**Description:** Inbound data error flag

**Syntax:** `INBOUND_DATA_ERROR`

**Implementation Status:** No

**Example:**

```apache
SecRule INBOUND_DATA_ERROR "@rx value" "id:1001,deny,msg:'Test'"
```
