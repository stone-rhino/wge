+++
title = "SecAuditLog"
weight = 51
+++

**Description:** Configure the path to the primary audit log file.


**Syntax:** `SecAuditLog /path/to/audit.log`


**Case Sensitive:** Yes

**Implemented:** No


This directive defines the path to the main audit log file where the engine will write detailed information about transactions.


**Example:**


```apache
SecAuditLog /var/log/modsec_audit.log
```
