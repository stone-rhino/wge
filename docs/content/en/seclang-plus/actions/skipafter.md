+++
title = "skipAfter"
weight = 43
+++

**Description:** Skip to a named marker

**Syntax:** `skipAfter:MARKER_NAME`

The skipAfter action jumps to a named marker (created with `SecMarker`) when the current rule matches. This provides flexible flow control for conditional rule execution, especially useful for complex rule sets.

Advantages over `skip`:
- More readable and maintainable
- Independent of rule count (can add/remove rules without breaking skip logic)
- Better for conditional bypassing of rule groups
- Clearer intent in configuration

**Example:**

```apache
# Skip all SQL injection checks for trusted paths
SecRule REQUEST_URI "^/api/internal/" \
    "id:100,phase:2,pass,nolog,skipAfter:END_SQLI_CHECKS"
SecRule ARGS "@detectSQLi" "id:101,phase:2,deny,status:403,msg:'SQL Injection'"
SecRule ARGS "@rx union.*select" "id:102,phase:2,deny,status:403,msg:'SQL Union'"
SecRule ARGS "@rx (\%27)|(\')" "id:103,phase:2,deny,status:403,msg:'SQL Quote'"
SecMarker END_SQLI_CHECKS

# Conditional authentication bypass
SecRule REQUEST_URI "^/public/" \
    "id:200,phase:1,pass,nolog,skipAfter:AFTER_AUTH"
SecRule REQUEST_HEADERS:X-API-Key "!@streq secret123" \
    "id:201,phase:1,deny,status:403,msg:'Invalid API Key'"
SecMarker AFTER_AUTH
```

**Parameter Type:** `string`

**Case Sensitive:** Yes
