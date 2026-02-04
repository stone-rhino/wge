+++
title = "log"
weight = 18
+++

**Description:** Log to error log

**Syntax:** `log | log:value`

The log action records the rule match to the error log. This is the default behavior for most rules, but can be explicitly specified when combined with other actions. The logged message includes rule ID, matched variable, and the configured msg value.

**Example:**

```apache
SecRule ARGS "@rx test" "id:1001,log,status:403"
```

**Parameter Type:** `none`

**Case Sensitive:** Yes
