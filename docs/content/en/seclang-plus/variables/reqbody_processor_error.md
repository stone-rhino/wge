+++
title = "REQBODY_PROCESSOR_ERROR"
weight = 60
+++

**Description:** Request body processor error

**Syntax:** `REQBODY_PROCESSOR_ERROR`

**Implementation Status:** No

**Example:**

```apache
SecRule REQBODY_PROCESSOR_ERROR "@rx value" "id:1001,deny,msg:'Test'"
```
