+++
title = "REQBODY_PROCESSOR"
weight = 59
+++

**Description:** Request body processor type

**Syntax:** `REQBODY_PROCESSOR`

REQBODY_PROCESSOR contains the name of the request body processor used for the current request, such as URLENCODED, MULTIPART, JSON, or XML.
When the `Content-Type` request header is `application/x-www-form-urlencoded` or `multipart/form-data`, WGE automatically sets this variable to `URLENCODED` or `MULTIPART` respectively. Users can also manually specify processors such as `JSON` via the `ctl:requestBodyProcessor` action.

**Example:**

```apache
# Configure different request body processors based on Content-Type
SecRule REQUEST_HEADERS:Content-Type "^(?:application(?:/soap\\+|/)|text/)xml" \
     "id:'200000',phase:1,t:none,t:lowercase,pass,nolog,ctl:requestBodyProcessor=XML"

SecRule REQUEST_HEADERS:Content-Type "^application/json" \
     "id:'200001',phase:1,t:none,t:lowercase,pass,nolog,ctl:requestBodyProcessor=JSON"
```
