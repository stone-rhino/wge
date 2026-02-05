+++
title = "SecAuditEngine"
weight = 50
+++

**Description:** Configure the audit logging engine.


**Syntax:** `SecAuditEngine On|Off|RelevantOnly`


**Default:** Off

**Options:**


- **On:** Enable audit logging for all transactions
- **Off:** Disable audit logging
- **RelevantOnly:** Only log transactions that trigger a warning/error or match the status code pattern configured via SecAuditLogRelevantStatus

**Case Sensitive:** Yes

**Implemented:** No


**Example:**


```apache
SecAuditEngine RelevantOnly
```
