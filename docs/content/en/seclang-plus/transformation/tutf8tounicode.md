+++
title = "t:utf8ToUnicode"
weight = 32
+++

**Description:** UTF-8 to Unicode

**Syntax:** `t:utf8ToUnicode`

Converts all UTF-8 character sequences to Unicode format ('%uHHHH'). This helps with input normalization, particularly for non-English languages, minimizing false positives and false negatives.

**Example:**

```apache
SecRule ARGS "@rx test" "id:1001,t:utf8ToUnicode"
```

**Parameter Type:** `string`
