+++
title = "SecArgumentsLimit"
weight = 10
+++

**Description:** Configure the maximum number of parameters that can be accepted.

**Syntax:** `SecArgumentsLimit LIMIT`

When using this setting, it is recommended to create a matching rule that detects the same integer value and rejects the request when the limit is reached. Without a matching rule, an attacker could bypass detection by placing attack payloads in parameters beyond the limit.

**Example:**

```
SecArgumentsLimit 1000
```

**Case Sensitive:** Yes
