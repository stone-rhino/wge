+++
title = "ARGS_GET_NAMES"
weight = 5
+++

**Description:** GET parameter names

**Syntax:** `ARGS_GET_NAMES`

ARGS_GET_NAMES contains only the names of parameters in the URL query string. Used to detect whether GET parameter names contain suspicious content without checking POST parameter names.

**Example:**

```apache
SecRule ARGS_GET_NAMES "@detectSQLi" "id:1007,phase:1,deny,msg:'SQL injection detected in GET parameter name'"
```
