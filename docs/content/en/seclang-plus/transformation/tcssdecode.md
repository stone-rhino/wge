+++
title = "t:cssDecode"
weight = 15
+++

**Description:** Decodes ``CSS`` escape sequences in a string.

**Syntax:** `t:cssDecode`

**Implemented:** Yes

**Example:**

```apache
# Detect CSS injection attacks
SecRule ARGS "@rx expression|javascript" \
    "id:1,phase:2,t:cssDecode,t:lowercase,deny,msg:'CSS injection detected'"

# Original input: \65\78\70\72\65\73\73\69\6f\6e (CSS-encoded "expression")
# After decoding: expression
```
