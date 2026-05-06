+++
title = "SecUploadFileLimit"
weight = 34
+++

**Description:** Configure the maximum number of files allowed to be uploaded in a single request.


**Syntax:** `SecUploadFileLimit INT`


**Default:** 0


**Case Sensitive:** Yes

When the number of uploaded files exceeds the configured value, the error flag MULTIPART_FILE_LIMIT_EXCEEDED will be set. This error value, along with other multipart/form-data format errors, is included in the variable MULTIPART_STRICT_ERROR.

**Example:**


```apache
SecUploadFileLimit 10
```
