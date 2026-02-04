+++
title = "expirevar"
weight = 26
+++

**Description:** Set variable expiration time

**Syntax:** `expirevar:collection.variable=seconds`

The expirevar action sets a time-to-live (TTL) for a collection variable. After the specified number of seconds, the variable is automatically removed. This is commonly used for implementing temporary bans, rate limiting windows, and session-based tracking.

**Example:**

```apache
SecRule ARGS "@rx attack" "id:1001,pass,setvar:ip.blocked=1,expirevar:ip.blocked=600"
```

**Parameter Type:** `string`

**Case Sensitive:** Yes
