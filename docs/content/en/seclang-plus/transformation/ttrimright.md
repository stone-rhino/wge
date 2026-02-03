+++
title = "t:trimRight"
weight = 38
+++

**Description:** Trim trailing whitespace

**Syntax:** `t:trimRight`

The t:trimRight transformation removes all whitespace characters from the right side (end) of the input string, including spaces, tabs, newlines, etc.

**Example:**

```apache
# Detect after trimming trailing whitespace from parameter
SecRule ARGS:cmd "@rx ;$" \
    "id:1,phase:2,t:trimRight,deny,msg:'Command separator detected'"

# Original input: "ls -la;   "
# After processing: "ls -la;"
```

**Parameter Type:** `none`

**Case Sensitive:** Yes
