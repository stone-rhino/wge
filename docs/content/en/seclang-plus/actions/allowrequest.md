+++
title = "allow:request"
weight = 14
+++

**Description:** Allow the entire request to pass

**Syntax:** `allow:request`

The allow:request action permits the entire request to bypass all remaining rules. This is useful for whitelisting known-good requests or trusted sources. Once triggered, no further rules in any phase will be evaluated for this request.

**Example:**

```apache
SecRule ARGS "@rx test" "id:1001,allow:request"
```

**Parameter Type:** `string`

**Case Sensitive:** Yes
