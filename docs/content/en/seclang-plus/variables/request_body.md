+++
title = "REQUEST_BODY"
weight = 12
+++

**Description:** Request body content

**Syntax:** `REQUEST_BODY`

REQUEST_BODY contains the raw request body data. Unlike ARGS_POST, REQUEST_BODY is unparsed raw data. When the request body processor cannot parse the content (such as unknown Content-Type), this variable can be used to inspect the raw data. SecRequestBodyAccess must be enabled to access this variable.

**Example:**

```apache
# Check raw request body for malicious patterns
SecRule REQUEST_BODY "@rx <script" \
    "id:1016,phase:2,deny,msg:'Script tag detected in request body'"
```
