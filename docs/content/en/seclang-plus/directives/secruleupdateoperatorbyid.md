+++
title = "SecRuleUpdateOperatorById"
weight = 62
+++

**Description:** Update the operator of an existing rule by its ID.


**Syntax:** `SecRuleUpdateOperatorById ID [ID...] "OPERATOR"`


**Case Sensitive:** Yes

**WGE Extension:** This is a WGE-specific extension, not available in standard ModSecurity.


This directive allows you to modify the operator of one or more existing rules by their ID. This is useful for customizing third-party rule sets (like OWASP CRS) without modifying the original rule files.

The ID can be a single rule ID, a range (e.g., `100-200`), or an ID with chain index (e.g., `100:1` for the first chained rule).


**Example:**


```apache
# Change rule 942100 to use case-insensitive matching
SecRuleUpdateOperatorById 942100 "@rx (?i)select.*from"

# Update multiple rules at once
SecRuleUpdateOperatorById 942100 942110 942120 "@pm select insert update delete"

# Update a rule range
SecRuleUpdateOperatorById 942100-942199 "@rx new_pattern"
```
