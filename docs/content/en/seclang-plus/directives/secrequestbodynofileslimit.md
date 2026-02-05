+++
title = "SecRequestBodyNoFilesLimit"
weight = 4
+++

**Description:** Configure the maximum request body size in bytes for requests without file uploads.


**Syntax:** `SecRequestBodyNoFilesLimit LIMIT`


**Default:** 1048576 (1MB)


**Case Sensitive:** Yes

**Implemented:** No


WGE can parse this directive correctly, but the functionality is not yet implemented.


**Example:**


```apache
SecRequestBodyNoFilesLimit 131072
```
