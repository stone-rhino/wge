+++
title = "SecAuditLogType"
weight = 59
+++

**Description:** Configure the type of audit logging mechanism.


**Syntax:** `SecAuditLogType Serial|Concurrent`


**Default:** Serial

**Options:**


- **Serial:** Write all audit log entries to a single file (specified by SecAuditLog), suitable for low-traffic environments
- **Concurrent:** Create separate files for each transaction in the directory specified by SecAuditLogStorageDir, suitable for high-traffic environments

**Case Sensitive:** Yes

**Implemented:** No


**Example:**


```apache
SecAuditLogType Concurrent
```
