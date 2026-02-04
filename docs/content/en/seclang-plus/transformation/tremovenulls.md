+++
title = "t:removeNulls"
weight = 18
+++

**Description:** Remove null characters

**Syntax:** `t:removeNulls`

Removes all NUL bytes from the input. This is useful for detecting attacks that use null byte injection to evade detection or terminate strings prematurely.

**Example:**

```apache
SecRule ARGS "@rx test" "id:1001,t:removeNulls"
```

**Parameter Type:** `string`
