+++
title = "initcol"
weight = 31
+++

**Description:** Initialize persistent collection

**Syntax:** `initcol:collection=key`

The `initcol` action initializes persistent collections (`IP`, `SESSION`, `USER`, `RESOURCE`, `GLOBAL`). These collections are commonly used for IP rate limiting, session tracking, and user behavior analysis.

**Available Collections:**

- **IP** - Persistent storage based on client IP address
- **SESSION** - Persistent storage based on session ID
- **USER** - Persistent storage based on user ID
- **RESOURCE** - Persistent storage based on resource identifier
- **GLOBAL** - Global persistent storage

At the moment, WGE can parse this action but does not implement real persistence. In practice, it behaves similarly to using `setvar` variables within the transaction lifecycle.

**Case Sensitive:** Yes

**Note:** This action supports execution control prefixes. You can force execution when a rule does not match (`!initcol`) or always execute regardless of match result (`*initcol`).

**Example:**

```apache
# Initialize IP collection for rate limiting
SecAction "id:1,phase:1,pass,nolog,initcol:IP=%{REMOTE_ADDR}"

# Execute initcol when the rule does not match
SecRule ARGS "@rx admin" "id:2,phase:1,pass,nolog,!initcol:IP=%{REMOTE_ADDR}"

# Always execute initcol regardless of match result
SecRule ARGS "@rx admin" "id:3,phase:1,pass,nolog,*initcol:IP=%{REMOTE_ADDR}"
```
