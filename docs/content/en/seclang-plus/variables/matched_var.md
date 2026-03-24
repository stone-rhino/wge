+++
title = "MATCHED_VAR"
weight = 87
+++

**Description:** Last matched variable value

**Syntax:** `MATCHED_VAR`

MATCHED_VAR contains the value of the last variable that matched successfully in the rule. When a rule checks multiple variables (e.g., ARGS) and finds a match, this variable holds the specific value that triggered the match. Commonly used for logging and debugging to understand exactly what content triggered the rule.

**Note:** By default, `MATCHED_VAR` returns the last value matched by the parent rule. If there is no parent rule, it returns the last value matched by the current rule.

**Example:**

```apache
# Log the matched content
SecRule ARGS "@detectSQLi" \
    "id:1063,phase:2,deny,msg:'SQL injection: %{MATCHED_VAR}'"
```
