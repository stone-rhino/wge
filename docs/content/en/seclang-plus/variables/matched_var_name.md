+++
title = "MATCHED_VAR_NAME"
weight = 88
+++

**Description:** Last matched variable name

**Syntax:** `MATCHED_VAR_NAME`

MATCHED_VAR_NAME contains the full name of the last variable that matched successfully. For example, if ARGS:username triggered the rule, the value of MATCHED_VAR_NAME would be "ARGS:username". Used together with MATCHED_VAR to fully record both the location and content that triggered the rule.

**Note:** By default, `MATCHED_VAR_NAME` returns the full name matched by the parent rule. If there is no parent rule, it returns the full name matched by the current rule.

**Example:**

```apache
# Log the matched variable name and value
SecRule ARGS "@detectXSS" \
    "id:1064,phase:2,deny,msg:'XSS in %{MATCHED_VAR_NAME}: %{MATCHED_VAR}'"
```
