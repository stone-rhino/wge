+++
title = "t:removeComments"
weight = 21
+++

**Description:** Remove comments

**Syntax:** `t:removeComments`

Removes all comment sequences (/* ... */, --, #) from the input. Multiple consecutive comment markers are not compressed. This is useful for detecting SQL injection and other attacks that use comments to bypass filters.

**Example:**

```apache
SecRule ARGS "@rx test" "id:1001,t:removeComments"
```

**Parameter Type:** `string`
