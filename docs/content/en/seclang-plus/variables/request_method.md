+++
title = "REQUEST_METHOD"
weight = 14
+++

**Description:** Request method (GET, POST, etc.)

**Syntax:** `REQUEST_METHOD`

REQUEST_METHOD contains the HTTP request method, such as GET, POST, PUT, DELETE, HEAD, OPTIONS, etc. Used to restrict allowed request methods for specific endpoints or detect abnormal HTTP methods.

**Example:**

```apache
# Allow only GET and POST methods
SecRule REQUEST_METHOD "!@pm GET POST HEAD" \
    "id:1018,phase:1,deny,msg:'HTTP method not allowed'"

# Block TRACE method (prevent XST attacks)
SecRule REQUEST_METHOD "@streq TRACE" \
    "id:1019,phase:1,deny,msg:'TRACE method is forbidden'"
```
