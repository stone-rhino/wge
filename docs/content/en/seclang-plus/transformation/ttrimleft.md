+++
title = "t:trimLeft"
weight = 37
+++

**Description:** Removes leading whitespace from a string.

This transformation function removes ``\x20\t\n\r\f\v`` characters from the beginning of the input string.

**Syntax:** `t:trimLeft`

**Implemented:** Yes

**Example:**

```apache
# Detect after trimming leading whitespace from parameter
SecRule ARGS:username "@rx ^admin" \
    "id:1,phase:2,t:trimLeft,deny,msg:'admin username detected'"

# Original input: "   admin"
# After processing: "admin"
```
