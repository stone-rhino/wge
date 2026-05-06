+++
title = "t:lowercase"
weight = 2
+++

**Description:** Converts uppercase characters to lowercase in a string.

**Syntax:** `t:lowercase`

**Implemented:** Yes

**Example:**

```apache
SecRule ARGS "@rx test" "id:1001,t:lowercase"
```
