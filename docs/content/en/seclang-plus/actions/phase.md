+++
title = "phase"
weight = 2
+++

**Description:** Processing phase (1-5)

**Syntax:** `phase:1|2|3|4|5|request|response|logging`

The phase action specifies which processing phase a rule executes in. WGE processes HTTP transactions through five sequential phases, each triggered when specific data becomes available. Selecting the correct phase is crucial for rule effectiveness.

- **phase:1 (request)** - Request headers phase: Executes immediately after receiving complete request headers, before the request body is read. Used for early decisions such as IP blacklisting and URL detection.
- **phase:2 (request)** - Request body phase: Executes after the request body is fully received and parsed. Most application-layer rules run in this phase, with access to ARGS, REQUEST_BODY and other variables.
- **phase:3 (response)** - Response headers phase: Executes after receiving backend response headers, before the response body is read. Used for checking response status codes and headers.
- **phase:4 (response)** - Response body phase: Executes after the response body is fully received. Used for detecting sensitive data leakage in response content.
- **phase:5 (logging)** - Logging phase: Executes after the response is sent, cannot block requests. Used only for logging and statistics.

**Example:**

```apache
# Detect IP blacklist in request headers phase
SecRule REMOTE_ADDR "@ipMatch 1.2.3.4" "id:1001,phase:1,deny,msg:'IP Blacklisted'"

# Detect SQL injection in request body phase
SecRule ARGS "@detectSQLi" "id:1002,phase:2,deny,msg:'SQL Injection Detected'"

# Detect sensitive data leakage in response body phase
SecRule RESPONSE_BODY "@rx \d{16}" "id:1003,phase:4,deny,msg:'Credit Card Number Leaked'"
```

**Parameter Type:** `int or string`

**Case Sensitive:** Yes
