+++
title = "t:removeCommentsChar"
weight = 22
+++

**Description:** Remove comment characters

**Syntax:** `t:removeCommentsChar`

Removes common comment characters (/* , */, --, #). This is useful for detecting SQL injection and other attacks that use comment sequences to bypass filters.

**Example:**

```apache
SecRule ARGS "@rx test" "id:1001,t:removeCommentsChar"
```

**Parameter Type:** `string`
