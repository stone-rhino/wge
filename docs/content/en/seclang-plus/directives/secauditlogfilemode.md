+++
title = "SecAuditLogFileMode"
weight = 55
+++

**Description:** Configure the file permission mode for audit log files.


**Syntax:** `SecAuditLogFileMode OCTAL`


**Default:** 0600

**Case Sensitive:** Yes

**Implemented:** No


This directive sets the Unix permission mode for audit log files created when using concurrent audit logging. The value must be specified in octal format.


**Example:**


```apache
SecAuditLogFileMode 0640
```
