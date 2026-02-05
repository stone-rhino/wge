+++
title = "skip"
weight = 42
+++

**Description:** Skip a specified number of rules

**Syntax:** `skip:N`

The skip action skips the next N rules in the configuration when the current rule matches. This provides a simple way to conditionally bypass rules without using chains or markers.

Important notes:
- N must be a positive integer
- Skip only works within the same phase
- Skip counts rules, not chains (each chained rule counts as one rule)
- For complex conditional logic, consider using `skipAfter` with `SecMarker` instead

**Example:**

```apache
# Skip next 2 rules if request is from trusted IP
SecRule REMOTE_ADDR "@ipMatch 10.0.0.0/8" \
    "id:100,phase:1,pass,nolog,skip:2"
SecRule ARGS "@detectSQLi" "id:101,phase:1,deny,status:403,msg:'SQL Injection'"
SecRule ARGS "@detectXSS" "id:102,phase:1,deny,status:403,msg:'XSS Attack'"

# Skip authentication check for static resources
SecRule REQUEST_URI "\.(css|js|png|jpg)$" \
    "id:200,phase:1,pass,nolog,skip:1"
SecRule REQUEST_HEADERS:Authorization "!@rx ^Bearer " \
    "id:201,phase:1,deny,status:401,msg:'Missing auth token'"
```

**Parameter Type:** `integer`

**Case Sensitive:** Yes
