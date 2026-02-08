+++
title = "MODSEC_BUILD"
weight = 74
+++

**Description:** ModSecurity build version

**Syntax:** `MODSEC_BUILD`

**Implementation Status:** No

**Example:**

```apache
SecRule MODSEC_BUILD "@rx value" "id:1001,deny,msg:'Test'"
```
