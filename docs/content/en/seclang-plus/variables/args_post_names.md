+++
title = "ARGS_POST_NAMES"
weight = 6
+++

**Description:** POST parameter names

**Syntax:** `ARGS_POST_NAMES`

ARGS_POST_NAMES contains only the names of parameters in the request body. Used to detect whether POST parameter names contain suspicious content. SecRequestBodyAccess must be enabled to access this variable.

**Example:**

```apache
SecRule ARGS_POST_NAMES "@detectXSS" "id:1008,phase:2,deny,msg:'XSS detected in POST parameter name'"
```
