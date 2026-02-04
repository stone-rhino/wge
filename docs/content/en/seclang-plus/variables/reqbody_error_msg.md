+++
title = "REQBODY_ERROR_MSG"
weight = 58
+++

**Description:** Request body error message

**Syntax:** `REQBODY_ERROR_MSG`

REQBODY_ERROR_MSG contains the error description when request body parsing fails. Typically used together with REQBODY_ERROR to log the specific error reason for debugging and analysis. Can be referenced in the msg action via %{REQBODY_ERROR_MSG}.

**Example:**

```apache
# Log detailed request body parsing error information
SecRule REQBODY_ERROR "!@eq 0" \
    "id:1045,phase:2,pass,log,msg:'Request body error: %{REQBODY_ERROR_MSG}'"
```
