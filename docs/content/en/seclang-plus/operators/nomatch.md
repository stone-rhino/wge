+++
title = "@noMatch"
weight = 32
+++

**Description:** No match (always returns false)

**Syntax:** `"@noMatch"`

@noMatch unconditionally returns no match (false). The rule will never trigger its action. This operator is primarily used for debugging purposes to temporarily disable rules without commenting or deleting rule code. Can also be used for placeholder rules or temporarily disabling specific detection in test environments.

**Example:**

```apache
# Temporarily disable rule (for debugging)
SecRule ARGS "@noMatch" \
    "id:1119,phase:2,deny,msg:'This rule is disabled'"

# Create placeholder rule, reserve rule ID
SecRule REQUEST_URI "@noMatch" \
    "id:1120,phase:1,deny,msg:'Reserved rule ID - to be implemented'"

# Disable certain detection in test environment
SecRule TX:testing_mode "@eq 1" \
    "id:1121,phase:1,chain,pass"
    SecRule ARGS "@noMatch" \
        "deny,msg:'This detection is disabled in test mode'"
```

**Parameter Type:** `none`

**Case Sensitive:** Yes
