+++
title = "SecUploadDir"
weight = 33
+++

**Description:** Configure the storage directory for file uploads.

**Syntax:** `SecUploadDir PATH`

**Case Sensitive:** Yes

**Implemented:** No

This directory must be on the same filesystem as the temporary directory defined by SecTmpDir. This directive should be used in conjunction with SecUploadKeepFiles.

This directive is not implemented in WGE and cannot be used.

**Example:**

```apache
SecUploadDir /var/lib/wge/upload/
```
