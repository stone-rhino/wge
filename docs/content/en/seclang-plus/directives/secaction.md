+++
title = "SecAction"
weight = 38
+++

**Description:** Unconditionally execute the specified actions.

**Syntax:** `SecAction "ACTIONS"`

**Version:** 1.0

SecAction is used to unconditionally execute a set of actions, commonly used for setting variables, initializing collections, etc.

SecAction unconditionally processes the specified action list, and its parameter syntax is identical to the third parameter of SecRule. This directive is commonly used for setting variables and initializing persistent collections through the initcol action. Because there are no variable and operator parameters, SecAction's actions will be executed unconditionally during each processing.

**Example:**

```apache
SecAction "id:9001,phase:1,nolog,pass,setvar:tx.blocking_paranoia_level=2"
```

**Case Sensitive:** Yes
