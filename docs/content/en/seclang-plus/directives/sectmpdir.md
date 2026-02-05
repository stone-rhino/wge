+++
title = "SecTmpDir"
weight = 14
+++

**Description:** Configure the temporary file storage directory.


**Syntax:** `SecTmpDir PATH`


**Case Sensitive:** Yes

**Implemented:** No

This directory is used to store temporary files during request body processing, especially when the request body size exceeds the SecRequestBodyInMemoryLimit setting. The WAF will write the excess portion to this directory. The directory must have write permissions for the user running the WAF process.

WGE can parse this directive correctly, but the functionality is not yet implemented.

**Example:**


```apache
SecTmpDir /tmp/
```
