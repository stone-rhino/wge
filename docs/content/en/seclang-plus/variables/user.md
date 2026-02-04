+++
title = "USER"
weight = 85
+++

**Description:** User variable collection

**Syntax:** `USER:key`

The USER collection is used to store persistent data associated with authenticated users. You need to first set the user identifier via the setuid action and initialize it. Can be used to track specific user behavior patterns and implement user-level access control.

**Example:**

```apache
# Set user ID
SecRule ARGS_POST:username "@rx (.+)" \
    "id:1061,phase:2,pass,nolog,setuid:%{TX.1}"
```
