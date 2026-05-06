+++
title = "accuracy"
weight = 8
+++

**Description:** Accuracy level (1-9)

**Syntax:** `accuracy:level`

The accuracy action specifies the accuracy level of a rule on a scale of 1 to 9. Higher values indicate rules with fewer false positives. This metadata helps operators understand the reliability of different rules and make informed decisions about which rules to enable in production.

**Example:**

```apache
SecRule ARGS "@rx test" "id:1001,deny,accuracy:8"
```

**Parameter Type:** `int`

**Case Sensitive:** Yes
