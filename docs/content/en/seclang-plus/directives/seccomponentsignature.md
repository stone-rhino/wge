+++
title = "SecComponentSignature"
weight = 31
+++

**Description:** Add component signature information to the log.


**Syntax:** `SecComponentSignature "SIGNATURE"`


**Case Sensitive:** Yes


**Implemented:** No


This directive is used to identify the presence of important rule sets. The complete signature will be recorded in the transaction audit log.


**Example:**


```apache
SecComponentSignature "OWASP_CRS/3.3.2"
```
