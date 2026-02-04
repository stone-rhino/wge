+++
title = "t:cmdline"
weight = 10
+++

**Description:** Command line argument normalization

**Syntax:** `t:cmdline`

The t:cmdline transformation normalizes command line arguments for detecting command injection attacks. On Windows and Unix systems, commands can be escaped in various ways (using quotes, backslashes, carets, etc.). This transformation processes input by:

- Removing all backslashes (\)
- Removing all double quotes (")
- Removing all single quotes (')
- Removing all carets (^)
- Removing spaces before /
- Removing spaces before -
- Compressing redundant whitespace

**Example:**

```apache
# Detect command injection
SecRule ARGS "@rx (cat|ls|wget|curl|nc|bash)" \
    "id:1,phase:2,t:cmdline,t:lowercase,deny,msg:'Command injection detected'"

# Original input: c^a"t /e't'c/p\asswd
# After processing: cat /etc/passwd
```

**Parameter Type:** `none`

**Case Sensitive:** Yes
