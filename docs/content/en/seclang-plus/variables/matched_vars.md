+++
title = "MATCHED_VARS"
weight = 89
+++

**Description:** All matched variable values

**Syntax:** `MATCHED_VARS`

MATCHED_VARS is a collection variable containing all variable values that matched successfully in the current rule. Unlike MATCHED_VAR, when a rule matches multiple values (e.g., multiple parameters containing malicious content), MATCHED_VARS contains all these values. Can be used in chain rules to further inspect all matched content.

**Example:**

```apache
# Check the length of all matched values
SecRule ARGS "@rx script" \
    "id:1065,phase:2,pass,setvar:tx.script_found=1,chain"
SecRule MATCHED_VARS "@gt 100" "t:length,deny,msg:'Long script content detected'"
```
