+++
title = "SecPcreMatchLimit"
weight = 12
+++

**Description:** Configure the maximum number of PCRE regular expression matches to prevent ReDoS attacks.

**Syntax:** `SecPcreMatchLimit LIMIT`

**Default:** 3000

If the configured limit is exceeded, the MSC_PCRE_LIMITS_EXCEEDED variable will be set.

**Example:**

```
SecPcreMatchLimit 3000
```

**Case Sensitive:** Yes
