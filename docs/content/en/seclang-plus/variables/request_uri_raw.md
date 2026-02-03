+++
title = "REQUEST_URI_RAW"
weight = 16
+++

**Description:** Raw request URI (not decoded)

**Syntax:** `REQUEST_URI_RAW`

REQUEST_URI_RAW contains the original, non-URL-decoded request URI. Used to detect encoding bypass attacks where attackers may use multiple encoding or abnormal encoding to bypass WAF detection. It is recommended to check both REQUEST_URI and REQUEST_URI_RAW for more comprehensive protection.

**Example:**

```apache
# Detect double encoding attacks in URL
SecRule REQUEST_URI_RAW "@rx %25" \
    "id:1022,phase:1,deny,msg:'Double encoding detected'"

# Detect NULL byte injection
SecRule REQUEST_URI_RAW "@rx %00" \
    "id:1023,phase:1,deny,msg:'NULL byte injection detected'"
```
