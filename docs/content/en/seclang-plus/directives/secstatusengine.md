+++
title = "SecStatusEngine"
weight = 19
+++

**Description:** Configure the status engine for collecting and reporting runtime statistics.

**Syntax:** `SecStatusEngine On|Off`

**Default:** Off

When enabled, the status engine collects WAF runtime statistics including the number of requests processed, rule matches, blocks, and more. This information helps monitor WAF performance and security posture.

**Example:**

```
SecStatusEngine On
```

**Case Sensitive:** Yes
