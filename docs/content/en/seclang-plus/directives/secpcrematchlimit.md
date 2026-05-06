+++
title = "SecPcreMatchLimit"
weight = 12
+++

**Description:** Configure the maximum number of PCRE regular expression matches to prevent ReDoS attacks.


**Syntax:** `SecPcreMatchLimit LIMIT`


**Default:** 0


**Case Sensitive:** Yes


Since WGE currently uses re2 as the default regex matching library, PCRE is only used as a fallback when re2 fails to compile the regex, so this configuration has minimal impact.


**Example:**


```apache
SecPcreMatchLimit 3000
```
