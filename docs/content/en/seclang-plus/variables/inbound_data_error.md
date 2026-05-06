+++
title = "INBOUND_DATA_ERROR"
weight = 61
+++

**Description:** Request body length limit exceeded error

**Syntax:** `INBOUND_DATA_ERROR`

When the request body size exceeds the value configured by the `SecRequestBodyLimit` directive, this variable is set to `1`.

**Implementation Status:** No

WGE can parse this directive, but the functionality is not implemented yet.

**Example:**

```apache
SecRule INBOUND_DATA_ERROR "@rx value" "id:1001,deny,msg:'Test'"
```
