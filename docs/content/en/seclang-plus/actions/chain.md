+++
title = "chain"
weight = 40
+++

**Description:** Chain rules together

**Syntax:** `chain`

The chain action links rules together so they act as a single compound rule. When rules are chained, subsequent rules only execute if all preceding rules in the chain match. This allows complex multi-condition checks without duplicating code.

Key behaviors:
- Only the last rule in a chain can contain disruptive actions (deny, block, drop, etc.)
- Metadata actions (id, msg, tag, etc.) should be on the first rule of the chain
- If any rule in the chain fails to match, the entire chain stops processing
- Phase must be consistent across all chained rules

**Example:**

```apache
# Block requests to /admin from non-local IPs
SecRule REQUEST_URI "^/admin" \
    "id:100,phase:1,chain,deny,status:403,msg:'Admin access from non-local IP'"
    SecRule REMOTE_ADDR "!@ipMatch 127.0.0.1,10.0.0.0/8,192.168.0.0/16" ""

# Multi-condition check: POST to login with empty body
SecRule REQUEST_METHOD "@streq POST" \
    "id:101,phase:2,chain,deny,status:400,msg:'Empty POST to login'"
    SecRule REQUEST_URI "@beginsWith /login" "chain"
        SecRule REQUEST_BODY "@eq ''" ""
```

**Parameter Type:** None

**Case Sensitive:** Yes
