+++
title = "SecUploadFileMode"
weight = 35
+++

**Description:** Configure the permission mode for uploaded files (octal).

**Syntax:** `SecUploadFileMode MODE`

**Default:** 0600

This feature is not available on operating systems that do not support octal file modes. The default mode (0600) grants read and write permissions only to the account that wrote the file. If other accounts need access, this directive may be required. However, use it carefully to avoid exposing potentially sensitive data to unauthorized users.

**Example:**

```
SecUploadFileMode 0640
```

**Case Sensitive:** Yes
