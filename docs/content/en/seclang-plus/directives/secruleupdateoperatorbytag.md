+++
title = "SecRuleUpdateOperatorByTag"
weight = 63
+++

**Description:** Update the operator of existing rules by their tag.


**Syntax:** `SecRuleUpdateOperatorByTag "TAG" "OPERATOR"`


**Case Sensitive:** Yes

**WGE Extension:** This is a WGE-specific extension, not available in standard ModSecurity.


This directive allows you to modify the operator of all rules that have a matching tag. This is useful for applying a consistent operator change across multiple related rules in a rule set.


**Example:**


```apache
# Update all SQL injection rules to use a new pattern
SecRuleUpdateOperatorByTag "attack-sqli" "@rx (?i)(union|select|insert|update|delete)"

# Update all XSS detection rules
SecRuleUpdateOperatorByTag "attack-xss" "@rx <script[^>]*>"
```
