+++
title = "t:none"
weight = 1
+++

**Description:** Clear transformation chain

**Syntax:** `t:none`

The t:none transformation clears all previous transformation functions, resetting the transformation chain. This is useful when you need to override transformations inherited from SecDefaultAction or SecRule. After using t:none, only transformation functions explicitly specified after it will be applied.

**Example:**

```apache
# Assume default action includes t:lowercase,t:urlDecode
SecDefaultAction "phase:2,log,pass,t:lowercase,t:urlDecode"

# This rule inherits default transformations
SecRule ARGS "@rx admin" "id:1,deny,msg:'admin detected'"

# Use t:none to clear default transformations, apply only t:base64Decode
SecRule ARGS:encoded "@rx admin" \
    "id:2,t:none,t:base64Decode,deny,msg:'admin detected in Base64'"

# Apply no transformations at all
SecRule REQUEST_BODY "@rx ^\{" \
    "id:3,t:none,pass,setvar:tx.is_json=1"
```

**Parameter Type:** `none`

**Case Sensitive:** Yes
