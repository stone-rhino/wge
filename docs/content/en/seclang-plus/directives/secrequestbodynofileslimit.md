+++
title = "SecRequestBodyNoFilesLimit"
weight = 4
+++

**Description:** Configure the maximum request body size in bytes for requests without file uploads.

**Syntax:** `SecRequestBodyNoFilesLimit LIMIT`

**Default:** 1048576 (1MB)

**Case Sensitive:** Yes

**Implemented:** No

**Example:**

```apache
SecRequestBodyNoFilesLimit 131072
```
