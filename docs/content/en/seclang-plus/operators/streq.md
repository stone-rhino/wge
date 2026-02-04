+++
title = "@streq"
weight = 7
+++

**Description:** String equality

**Syntax:** `"@streq string"`

@streq checks if the variable value exactly equals the specified string. Matching is case sensitive. Compared to @rx "^exact$" regex, @streq performs better for exact string comparisons. For case-insensitive comparison, apply the lowercase transformation function first. Commonly used for exact value validation, flag checks, and similar scenarios.

**Example:**

```apache
# Exact match HTTP method
SecRule REQUEST_METHOD "@streq POST" \
    "id:1081,phase:1,pass,nolog,setvar:tx.is_post=1"

# Validate specific header value
SecRule REQUEST_HEADERS:X-Requested-With "@streq XMLHttpRequest" \
    "id:1082,phase:1,pass,nolog,setvar:tx.is_ajax=1"

# Case-insensitive comparison (using lowercase transform)
SecRule REQUEST_HEADERS:Accept "!@streq application/json" \
    "id:1083,phase:1,t:lowercase,deny,msg:'Only JSON requests accepted'"

# Detect specific user agent
SecRule REQUEST_HEADERS:User-Agent "@streq curl/7.68.0" \
    "id:1084,phase:1,log,pass,msg:'curl client detected'"
```

**Parameter Type:** `string`

**Case Sensitive:** Yes
