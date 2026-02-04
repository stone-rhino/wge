+++
title = "REQUEST_FILENAME"
weight = 17
+++

**Description:** Requested file path

**Syntax:** `REQUEST_FILENAME`

REQUEST_FILENAME contains the file path portion of the request URI, excluding the query string. This is a normalized path extracted from the URI, used to detect access to specific files or directories.

**Example:**

```apache
# Block access to admin directory
SecRule REQUEST_FILENAME "@beginsWith /admin" \
    "id:1024,phase:1,deny,msg:'Access to admin directory forbidden'"

# Block access to PHP configuration files
SecRule REQUEST_FILENAME "@endsWith .htaccess" \
    "id:1025,phase:1,deny,msg:'Access to .htaccess forbidden'"
```
