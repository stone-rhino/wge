+++
title = "@contains"
weight = 2
+++

**Description:** Contains the specified string

**Syntax:** `"@contains string"`

@contains checks if the variable value contains the specified substring. Matching is case sensitive. Compared to @rx, @contains performs better for simple substring searches as it avoids regex engine overhead.

**Example:**

```apache
# Detect if Content-Type contains multipart
SecRule REQUEST_HEADERS:Content-Type "@contains multipart" \
    "id:1068,phase:1,pass,nolog,ctl:requestBodyProcessor=MULTIPART"

# Detect if URL contains admin
SecRule REQUEST_URI "@contains admin" \
    "id:1069,phase:1,log,msg:'Accessing admin path'"
```

**Parameter Type:** `string`
