+++
title = "@unconditionalMatch"
weight = 31
+++

**Description:** Unconditional match (always returns true)

**Syntax:** `"@unconditionalMatch"`

@unconditionalMatch unconditionally returns a match (true). Does not check any variable value and always triggers the rule action. Commonly used for creating unconditional rules such as initializing variables, setting defaults, and logging all requests. Typically has the same effect as SecAction, but is useful in rule chains that require a variable.

**Example:**

```apache
# Initialize transaction variables
SecRule REQUEST_URI "@unconditionalMatch" \
    "id:1116,phase:1,pass,nolog,setvar:tx.anomaly_score=0"

# Log all requests
SecRule REMOTE_ADDR "@unconditionalMatch" \
    "id:1117,phase:1,pass,log,msg:'Request from: %{REMOTE_ADDR}'"

# Use in rule chain
SecRule REQUEST_METHOD "@streq POST" \
    "id:1118,phase:1,chain,pass"
    SecRule REQUEST_URI "@unconditionalMatch" \
        "setvar:tx.is_post_request=1"
```

**Parameter Type:** `none`

**Case Sensitive:** Yes
