+++
title = "status"
weight = 44
+++

**Description:** Set HTTP response status code

**Syntax:** `status:CODE`

The status action sets the HTTP response status code to be returned when a disruptive action (deny, block, drop) is triggered. This allows fine-grained control over error responses.

Common status codes:
- **400** - Bad Request (malformed input)
- **401** - Unauthorized (authentication required)
- **403** - Forbidden (access denied)
- **404** - Not Found (hide resource existence)
- **405** - Method Not Allowed
- **429** - Too Many Requests (rate limiting)
- **500** - Internal Server Error
- **503** - Service Unavailable

**Example:**

```apache
# Return 403 Forbidden for SQL injection attempts
SecRule ARGS "@detectSQLi" \
    "id:100,phase:2,deny,status:403,msg:'SQL Injection Blocked'"

# Return 401 for missing authentication
SecRule REQUEST_HEADERS:Authorization "@eq ''" \
    "id:101,phase:1,deny,status:401,msg:'Authentication Required'"

# Return 429 for rate limit exceeded
SecRule IP:request_count "@gt 100" \
    "id:102,phase:1,deny,status:429,msg:'Rate Limit Exceeded'"

# Return 404 to hide admin paths
SecRule REQUEST_URI "^/admin" "chain,id:103,phase:1,deny,status:404"
    SecRule REMOTE_ADDR "!@ipMatch 10.0.0.0/8" ""
```

**Parameter Type:** `integer`

**Case Sensitive:** Yes
