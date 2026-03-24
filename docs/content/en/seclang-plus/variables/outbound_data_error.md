+++
title = "OUTBOUND_DATA_ERROR"
weight = 62
+++

**Description:** Response body length limit exceeded error

**Syntax:** `OUTBOUND_DATA_ERROR`

When the response body size exceeds the value configured by the `SecResponseBodyLimit` directive, this variable is set to `1`.

**Implementation Status:** No

WGE can parse this directive, but the functionality is not implemented yet.

**Example:**

```apache
SecRule OUTBOUND_DATA_ERROR "@rx value" "id:1001,deny,msg:'Test'"
```
