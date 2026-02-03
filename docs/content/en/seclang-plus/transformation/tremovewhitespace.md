+++
title = "t:removeWhitespace"
weight = 19
+++

**Description:** Remove whitespace

**Syntax:** `t:removeWhitespace`

Removes all whitespace characters from the input. This is useful for detecting attacks that use whitespace to break up keywords or evade pattern matching.

**Example:**

```apache
SecRule ARGS "@rx test" "id:1001,t:removeWhitespace"
```

**Parameter Type:** `string`
