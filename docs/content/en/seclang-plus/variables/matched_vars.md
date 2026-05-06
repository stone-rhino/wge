+++
title = "MATCHED_VARS"
weight = 89
+++

**Description:** All matched variable values

**Syntax:** `MATCHED_VARS`

MATCHED_VARS is a collection variable containing all variable values that matched successfully in the current rule. Unlike MATCHED_VAR, when a rule matches multiple values (e.g., multiple parameters containing malicious content), MATCHED_VARS contains all these values. Can be used in chain rules to further inspect all matched content.

**Note:** By default, `MATCHED_VARS` returns the values matched by the parent rule. If there is no parent rule, it returns the values matched by the current rule.

WGE's `MATCHED_VARS` behavior is not the same as ModSecurity. WGE does not keep accumulating matches indefinitely. It can return only the values matched by the parent rule or the current rule, as in the following example:

```apache
SecRule ARGS "@rx [0-9]+['\"]\s*(and|or)" "id:1001,phase:2,t:none,deny,chain"
    SecRule ARGS "@rx --" "chain,t:none"
        SecRule MATCHED_VARS "@rx select" "t:none"
```

In WGE, `MATCHED_VARS` in this rule gets only the value matched by the parent rule. In ModSecurity, it gets the values matched by both the parent rule and the grandparent rule. `MATCHED_VARS_NAMES` behaves similarly.

**Example:**

```apache
# Check the length of all matched values
SecRule ARGS "@rx script" \
    "id:1065,phase:2,pass,setvar:tx.script_found=1,chain"
SecRule MATCHED_VARS "@gt 100" "t:length,deny,msg:'Long script content detected'"
```
