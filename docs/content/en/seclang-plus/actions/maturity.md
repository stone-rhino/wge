+++
title = "maturity"
weight = 9
+++

**Description:** Maturity level (1-9)

**Syntax:** `maturity:level`

The maturity action specifies the maturity level of a rule on a scale of 1 to 9. Higher values indicate rules that have been more thoroughly tested and are considered production-ready. This metadata helps operators understand the stability and reliability of different rules.

**Example:**

```apache
SecRule ARGS "@rx test" "id:1001,deny,maturity:9"
```

**Parameter Type:** `int`

**Case Sensitive:** Yes
