+++
title = "SecResponseBodyLimit"
weight = 9
+++

**Description:** Configure the maximum response body buffer size in bytes.


**Syntax:** `SecResponseBodyLimit LIMIT`


**Default:** 524288 (512KB)


**Case Sensitive:** Yes

**Implemented:** No


WGE can parse this directive correctly, but the functionality is not yet implemented.


**Example:**


```apache
SecResponseBodyLimit 524288
```
