+++
title = "REQBODY_PROCESSOR_ERROR"
weight = 60
+++

**Description:** Request body processor error

**Syntax:** `REQBODY_PROCESSOR_ERROR`

**Example:**

```apache
SecRule REQBODY_PROCESSOR_ERROR "@rx value" "id:1001,deny,msg:'Test'"
```

**Implementation Status:** Not yet implemented
