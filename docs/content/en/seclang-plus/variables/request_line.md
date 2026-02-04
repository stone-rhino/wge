+++
title = "REQUEST_LINE"
weight = 20
+++

**Description:** Complete request line

**Syntax:** `REQUEST_LINE`

REQUEST_LINE contains the complete HTTP request line in the format "METHOD URI PROTOCOL", for example "GET /index.html HTTP/1.1". Used for comprehensive inspection of abnormal content in the request line.

**Example:**

```apache
# Detect control characters in request line
SecRule REQUEST_LINE "@rx [\x00-\x08\x0b\x0c\x0e-\x1f]" \
    "id:1030,phase:1,deny,msg:'Request line contains control characters'"

# Detect excessively long request line
SecRule REQUEST_LINE "@gt 8192" "t:length,id:1031,phase:1,deny,msg:'Request line too long'"
```
