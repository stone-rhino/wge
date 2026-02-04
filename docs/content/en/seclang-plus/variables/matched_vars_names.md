+++
title = "MATCHED_VARS_NAMES"
weight = 90
+++

**Description:** All matched variable names

**Syntax:** `MATCHED_VARS_NAMES`

MATCHED_VARS_NAMES is a collection variable containing the names of all variables that matched successfully in the current rule. Unlike MATCHED_VAR_NAME, when a rule matches multiple variables, this variable contains all matched variable names.

**Example:**

```apache
# Log all matched variable names
SecRule ARGS|REQUEST_HEADERS "@detectSQLi" \
    "id:1066,phase:2,deny,msg:'SQL injection in: %{MATCHED_VARS_NAMES}'"
```
