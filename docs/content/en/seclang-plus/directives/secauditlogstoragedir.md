+++
title = "SecAuditLogStorageDir"
weight = 58
+++

**Description:** Configure the directory for storing individual audit log files in concurrent mode.


**Syntax:** `SecAuditLogStorageDir /path/to/dir`


**Case Sensitive:** Yes

**Implemented:** No


This directive specifies the directory where individual audit log files will be stored when using concurrent audit logging (SecAuditLogType Concurrent). Each transaction will have its own file in this directory.


**Example:**


```apache
SecAuditLogStorageDir /var/log/modsec_audit
```
