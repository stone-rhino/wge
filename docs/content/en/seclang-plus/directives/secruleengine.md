+++
title = "SecRuleEngine"
weight = 1
+++

**Description:** Configure the operating mode of the rule engine.


**Syntax:** `SecRuleEngine On|Off|DetectionOnly`


**Default:** Off

**Options:**


- **On:** Enable the rule engine, execute all rules and apply disruptive actions
- **Off:** Disable the rule engine, no rules are executed
- **DetectionOnly:** Detection-only mode, execute rules but do not apply disruptive actions

**Case Sensitive:** Yes



**Example:**


```apache
SecRuleEngine On
```
