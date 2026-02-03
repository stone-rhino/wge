+++
title = "t:cssDecode"
weight = 15
+++

**Description:** CSS decode

**Syntax:** `t:cssDecode`

The t:cssDecode transformation decodes CSS escape sequences. CSS allows using a backslash followed by hexadecimal digits (up to 6 digits) to represent characters. Attackers may exploit this feature to bypass detection. This transformation normalizes these CSS-encoded characters.

**Example:**

```apache
# Detect CSS injection attacks
SecRule ARGS "@rx expression|javascript" \
    "id:1,phase:2,t:cssDecode,t:lowercase,deny,msg:'CSS injection detected'"

# Original input: \65\78\70\72\65\73\73\69\6f\6e (CSS-encoded "expression")
# After decoding: expression
```

**Parameter Type:** `none`

**Case Sensitive:** Yes
