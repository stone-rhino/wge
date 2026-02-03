+++
title = "drop"
weight = 11
+++

**Description:** Drop connection

**Syntax:** `drop | drop:value`

The drop action immediately terminates the TCP connection without sending a response. This is more aggressive than deny and is typically used for severe attacks where you want to minimize resource usage and avoid any response to the attacker.

**Example:**

```apache
SecRule ARGS "@rx test" "id:1001,drop,status:403"
```

**Parameter Type:** `none`

**Case Sensitive:** Yes
