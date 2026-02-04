+++
title = "allow"
weight = 12
+++

**Description:** Allow request to pass

**Syntax:** `allow | allow:value`

The allow action permits the current request to pass without blocking. When used alone, it stops rule processing for the current phase and allows the transaction to continue. This is typically used for whitelisting trusted requests or implementing exceptions.

**Example:**

```apache
SecRule ARGS "@rx test" "id:1001,allow,status:403"
```

**Parameter Type:** `none or string`

**Case Sensitive:** Yes
