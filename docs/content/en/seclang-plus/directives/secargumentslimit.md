+++
title = "SecArgumentsLimit"
weight = 10
+++

**Description:** Configure the maximum number of acceptable parameters.


**Syntax:** `SecArgumentsLimit LIMIT`


**Case Sensitive:** Yes



**Implemented:** No


When using this setting, it is recommended to combine it with rules that detect the same integer value and reject requests when the limit is reached. For example:
If a matching rule is not configured, an attacker may evade detection by placing attack payloads in parameters beyond the limit.


**Example:**


```apache
SecArgumentsLimit 1000
```
