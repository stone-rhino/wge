+++
title = "REQBODY_ERROR"
weight = 57
+++

**Description:** Request body error flag

**Syntax:** `REQBODY_ERROR`

REQBODY_ERROR is set to 1 when errors occur during request body parsing. For example, when JSON format is invalid, XML format is incorrect, or multipart data is abnormal, this variable will be set. Used to detect and block requests with invalid request bodies in phase:2.

**Example:**

```apache
# Reject requests with failed body parsing
SecRule REQBODY_ERROR "!@eq 0" \
    "id:1044,phase:2,deny,status:400,msg:'Request body parsing failed: %{REQBODY_ERROR_MSG}'"
```
