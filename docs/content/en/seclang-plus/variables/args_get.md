+++
title = "ARGS_GET"
weight = 2
+++

**Description:** GET request parameters

**Syntax:** `ARGS_GET | ARGS_GET:Key`

ARGS_GET contains only parameters passed in the URL query string (i.e., GET parameters). Unlike ARGS, it does not include parameters from the POST request body, making it useful for inspecting only URL-passed data without false positives from POST data. Specific parameters can be accessed via ARGS_GET:paramname.

**Example:**

```apache
# Check for SQL injection in URL query parameters
SecRule ARGS_GET "@detectSQLi" "id:1001,phase:1,deny,status:403,msg:'SQL injection detected in GET parameters'"

# Check specific GET parameter
SecRule ARGS_GET:id "@rx [^0-9]" "id:1002,phase:1,deny,msg:'ID parameter must be numeric'"
```
