+++
title = "SecPcreMatchLimitRecursion"
weight = 13
+++

**Description:** Configure the PCRE regular expression recursion depth limit.

**Syntax:** `SecPcreMatchLimitRecursion LIMIT`

**Default:** 1000

This directive limits the recursion call depth during regular expression matching to prevent stack overflow caused by complex regular expressions. When a regular expression contains nested groups or backtracking, the recursion depth can increase rapidly. If the limit is reached, matching will terminate and the MSC_PCRE_LIMITS_EXCEEDED variable will be set.

**Example:**

```
SecPcreMatchLimitRecursion 1000
```

**Case Sensitive:** Yes
