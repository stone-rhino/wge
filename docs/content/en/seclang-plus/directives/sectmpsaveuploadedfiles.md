+++
title = "SecTmpSaveUploadedFiles"
weight = 32
+++

**Description:** Configure whether to save uploaded files to the temporary directory.

**Syntax:** `SecTmpSaveUploadedFiles On|Off`

**Default:** Off

**Case Sensitive:** Yes

**Implemented:** No

When enabled, files uploaded via multipart/form-data will be saved to the temporary directory specified by SecTmpDir. This allows rules to use the @inspectFile operator for deep inspection of uploaded files (such as virus scanning).

**Example:**

```apache
SecTmpSaveUploadedFiles On
```
