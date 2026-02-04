+++
title = "REQUEST_HEADERS"
weight = 8
+++

**Description:** Request headers

**Syntax:** `REQUEST_HEADERS | REQUEST_HEADERS:HeaderName`

REQUEST_HEADERS contains all HTTP request headers. Specific headers can be accessed using REQUEST_HEADERS:HeaderName, such as User-Agent, Host, Content-Type, etc. Request headers are common injection points for attackers, so key headers should be inspected.

**Example:**

```apache
# Check for malicious patterns in User-Agent
SecRule REQUEST_HEADERS:User-Agent "@pm nikto sqlmap nmap" \
    "id:1009,phase:1,deny,msg:'Scanning tool User-Agent detected'"

# Validate Host header format
SecRule REQUEST_HEADERS:Host "!@rx ^[a-zA-Z0-9\.\-]+$" \
    "id:1010,phase:1,deny,msg:'Invalid Host header'"

# Check for XSS in Referer header
SecRule REQUEST_HEADERS:Referer "@detectXSS" \
    "id:1011,phase:1,deny,msg:'XSS detected in Referer header'"
```
