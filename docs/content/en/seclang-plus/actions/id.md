+++
title = "id"
weight = 1
+++

**Description:** Unique rule identifier (required)

**Syntax:** `id:number`

The id action assigns a unique numeric identifier to a rule. This is a mandatory action for every rule. Rule IDs are used for logging, debugging, and rule management. Each rule must have a unique ID within the configuration.

**Example:**

```apache
SecRule ARGS "@rx test" "id:1001,deny"
```

**Parameter Type:** `int`

**Case Sensitive:** Yes
