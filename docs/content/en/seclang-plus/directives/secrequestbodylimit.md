+++
title = "SecRequestBodyLimit"
weight = 3
+++

**Description:** Configure the maximum acceptable request body size in bytes.


**Syntax:** `SecRequestBodyLimit LIMIT`


**Default:** 134217728 (128MB)


**Case Sensitive:** Yes

**Implemented:** No


Requests exceeding this limit will be rejected with a 413 (Request Entity Too Large) status code. The hard upper limit is 1 GB.

WGE can parse this directive correctly, but the functionality is not yet implemented.


**Example:**


```apache
SecRequestBodyLimit 134217728
```
