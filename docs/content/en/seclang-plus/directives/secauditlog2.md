+++
title = "SecAuditLog2"
weight = 52
+++

**Description:** Configure the path to the secondary audit log file.


**Syntax:** `SecAuditLog2 /path/to/audit2.log`


**Case Sensitive:** Yes

**Implemented:** No


This directive defines the path to a secondary audit log file, typically used for concurrent logging or as a backup destination.


**Example:**


```apache
SecAuditLog2 /var/log/modsec_audit2.log
```
