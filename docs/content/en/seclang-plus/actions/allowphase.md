+++
title = "allow:phase"
weight = 13
+++

**Description:** Allow request to pass current phase

**Syntax:** `allow:phase`

The allow:phase action permits the request to bypass remaining rules in the current processing phase only. Rules in subsequent phases will still be evaluated. This provides finer-grained control compared to allow:request. To allow a response, add a rule in the RESPONSE_HEADERS phase and use allow.

**Example:**

```apache
SecRule ARGS "@rx test" "id:1001,allow:phase"
```

**Parameter Type:** `string`

**Case Sensitive:** Yes
