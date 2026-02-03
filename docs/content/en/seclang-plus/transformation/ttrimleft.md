+++
title = "t:trimLeft"
weight = 37
+++

**Description:** Trim leading whitespace

**Syntax:** `t:trimLeft`

The t:trimLeft transformation removes all whitespace characters from the left side (beginning) of the input string, including spaces, tabs, newlines, etc.

**Example:**

```apache
# Detect after trimming leading whitespace from parameter
SecRule ARGS:username "@rx ^admin" \
    "id:1,phase:2,t:trimLeft,deny,msg:'admin username detected'"

# Original input: "   admin"
# After processing: "admin"
```

**Parameter Type:** `none`

**Case Sensitive:** Yes
