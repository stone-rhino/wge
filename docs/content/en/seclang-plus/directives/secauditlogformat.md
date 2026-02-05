+++
title = "SecAuditLogFormat"
weight = 54
+++

**Description:** Configure the format of the audit log output.


**Syntax:** `SecAuditLogFormat Native|JSON`


**Default:** Native

**Options:**


- **Native:** Use the traditional ModSecurity audit log format
- **JSON:** Output audit log entries in JSON format for easier parsing

**Case Sensitive:** Yes

**Implemented:** No


**Example:**


```apache
SecAuditLogFormat JSON
```
