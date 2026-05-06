+++
title = "multiMatch"
weight = 24
+++

**Description:** Execute action for all matches

**Syntax:** `multiMatch | multiMatch:value`

The multiMatch action causes the rule to be evaluated against each element of a collection variable individually. Without multiMatch, a rule checking ARGS would stop after the first matching argument. With multiMatch, all matching arguments are processed, which is useful when you want to capture or log all offending values.

**Example:**

```apache
SecRule ARGS "@rx test" "id:1001,multiMatch,status:403"
```

**Parameter Type:** `none`

**Case Sensitive:** Yes
