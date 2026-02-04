+++
title = "SecUploadDir"
weight = 33
+++

**Description:** Configure the storage directory for file uploads.

**Syntax:** `SecUploadDir PATH`

This directory must be on the same filesystem as the temporary directory defined by SecTmpDir. This directive should be used in conjunction with SecUploadKeepFiles.

**Example:**

```
SecUploadDir /var/lib/wge/upload/
```

**Case Sensitive:** Yes
