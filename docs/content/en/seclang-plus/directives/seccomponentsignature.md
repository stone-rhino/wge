+++
title = "SecComponentSignature"
weight = 31
+++

**Description:** Add component signature information to logs.

**Syntax:** `SecComponentSignature "SIGNATURE"`

This directive is used to identify the presence of important rule sets. The complete signature will be recorded in the transaction audit log.

**Example:**

```
SecComponentSignature "OWASP_CRS/3.3.2"
```

**Case Sensitive:** Yes
