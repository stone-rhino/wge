+++
title = "t:none"
weight = 1
+++

**Description:** Clears the transformation chain.

``t:none`` clears all transformation functions that appear before ``t:none`` in the transformation chain. After using ``t:none``, only transformation functions explicitly specified after it will be applied.

**Syntax:** `t:none`

**Implemented:** Yes

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
