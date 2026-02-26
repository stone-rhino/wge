+++
title = "t:trimRight"
weight = 38
+++

**Description:** Removes trailing whitespace from a string.

This transformation function removes ``\x20\t\n\r\f\v`` characters from the end of the input string.

**Syntax:** `t:trimRight`

**Implemented:** Yes

**Example:**

```apache
# Detect after trimming trailing whitespace from parameter
SecRule ARGS:cmd "@rx ;$" \
    "id:1,phase:2,t:trimRight,deny,msg:'Command separator detected'"

# Original input: "ls -la;   "
# After processing: "ls -la;"
```
