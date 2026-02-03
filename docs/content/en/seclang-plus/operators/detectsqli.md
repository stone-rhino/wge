+++
title = "@detectSQLi"
weight = 29
+++

**Description:** Detects SQL injection attacks using the libinjection library

**Syntax:** `"@detectSQLi"`

@detectSQLi uses libinjection's SQL injection detection engine to identify SQL injection patterns through lexical analysis and fingerprint matching. Compared to regex-based detection, libinjection provides lower false positive rates and higher detection accuracy. This operator requires no parameters.

**Example:**

```apache
# Detect SQL injection in all parameters
SecRule ARGS "@detectSQLi" \
    "id:1001,phase:2,deny,msg:'SQL Injection Attack Detected'"

# Combined with variable detection
SecRule ARGS|REQUEST_HEADERS "@detectSQLi" \
    "id:1002,phase:2,deny,severity:CRITICAL,\
    msg:'SQL Injection detected in %{MATCHED_VAR_NAME}'"
```

**Parameter Type:** `none`
