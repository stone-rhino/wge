+++
title = "REQBODY_PROCESSOR"
weight = 59
+++

**Description:** Request body processor type

**Syntax:** `REQBODY_PROCESSOR`

REQBODY_PROCESSOR contains the name of the request body processor used for the current request, such as URLENCODED, MULTIPART, JSON, or XML. This variable is automatically set by WGE based on Content-Type, and can also be manually specified via the ctl:requestBodyProcessor action.

**Example:**

```apache
# Execute different checks based on processor type
SecRule REQBODY_PROCESSOR "@streq JSON" \
    "id:1046,phase:2,pass,nolog,setvar:tx.json_request=1"
```
