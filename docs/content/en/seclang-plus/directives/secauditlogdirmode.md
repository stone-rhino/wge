+++
title = "SecAuditLogDirMode"
weight = 53
+++

**Description:** Configure the directory permission mode for concurrent audit log directories.


**Syntax:** `SecAuditLogDirMode OCTAL`


**Default:** 0600

**Case Sensitive:** Yes

**Implemented:** No


This directive sets the Unix permission mode for directories created when using concurrent audit logging. The value must be specified in octal format.


**Example:**


```apache
SecAuditLogDirMode 0750
```
