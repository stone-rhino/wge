+++
title = "t:cmdline"
weight = 10
+++

**Description:** Normalizes a string as command line arguments.

This transformation function processes the input string in the following ways:

- Remove all backslashes ``\``
- Remove all double quotes ``"``
- Remove all single quotes ``'``
- Remove all carets ``^``
- Remove spaces before slashes ``/``
- Remove spaces before left parentheses ``(``
- Replace commas ``,`` and semicolons ``;`` with spaces
- Compress consecutive whitespace (including tabs, newlines) into a single space
- Convert characters to lowercase

**Syntax:** `t:cmdline`

**Implemented:** Yes

**Example:**

```apache
# Detect command injection
SecRule ARGS "@rx (cat|ls|wget|curl|nc|bash)" \
    "id:1,phase:2,t:cmdline,t:lowercase,deny,msg:'Command injection detected'"

# Original input: c^a"t /e't'c/p\asswd
# After processing: cat /etc/passwd
```
