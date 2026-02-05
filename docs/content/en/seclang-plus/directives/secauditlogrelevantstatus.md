+++
title = "SecAuditLogRelevantStatus"
weight = 57
+++

**Description:** Configure which HTTP response status codes are considered relevant for audit logging.


**Syntax:** `SecAuditLogRelevantStatus REGEX`


**Case Sensitive:** Yes

**Implemented:** No


This directive defines a regular expression pattern to match HTTP response status codes. When SecAuditEngine is set to RelevantOnly, only transactions with matching response status codes will be logged.


**Example:**


```apache
SecAuditLogRelevantStatus "^(?:5|4(?!04))"
```

This example logs all 5xx errors and 4xx errors except 404.
