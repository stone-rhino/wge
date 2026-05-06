+++
title = "SecRequestBodyInMemoryLimit"
weight = 22
+++

**Description:** Configure the maximum size of request body buffered in memory; content exceeding this limit will be written to a temporary file.


**Syntax:** `SecRequestBodyInMemoryLimit LIMIT`


**Case Sensitive:** Yes

**Implemented:** No


This directive controls how much request body data the WAF keeps in memory. When the request body size exceeds this limit, the excess portion will be written to the temporary directory specified by SecTmpDir. Setting this value appropriately balances memory usage and I/O performance.

This directive is not implemented in WGE and cannot be used.


**Example:**


```apache
SecRequestBodyInMemoryLimit 131072
```
