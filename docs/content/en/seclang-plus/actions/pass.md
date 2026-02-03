+++
title = "pass"
weight = 16
+++

**Description:** Continue processing subsequent rules

**Syntax:** `pass | pass:value`

The pass action allows the transaction to continue to the next rule even when the current rule matches. This is a non-disruptive action commonly used in conjunction with setvar to record matches without blocking, enabling anomaly scoring and data collection patterns.

**Example:**

```apache
SecRule ARGS "@rx suspicious" "id:1001,pass,setvar:tx.anomaly_score=+3"
```

**Parameter Type:** `none`

**Case Sensitive:** Yes
