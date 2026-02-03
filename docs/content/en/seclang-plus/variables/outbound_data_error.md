+++
title = "OUTBOUND_DATA_ERROR"
weight = 62
+++

**Description:** Outbound data error flag

**Syntax:** `OUTBOUND_DATA_ERROR`

**Example:**

```apache
SecRule OUTBOUND_DATA_ERROR "@rx value" "id:1001,deny,msg:'Test'"
```

**Implementation Status:** Not yet implemented
