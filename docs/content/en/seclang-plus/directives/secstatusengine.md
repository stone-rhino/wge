+++
title = "SecStatusEngine"
weight = 19
+++

**Description:** Configure the status engine for collecting and reporting runtime statistics.


**Syntax:** `SecStatusEngine On|Off`


**Default:** Off


**Case Sensitive:** Yes

**Implemented:** No

When enabled, the status engine collects WAF runtime statistics including the number of requests processed, rule matches, blocks, and more. This information helps monitor WAF performance and security posture.

WGE can parse this directive correctly, but the functionality is not yet implemented.

**Example:**


```apache
SecStatusEngine On
```
