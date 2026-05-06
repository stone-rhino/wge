+++
title = "MODSEC_BUILD"
weight = 74
+++

**Description:** ModSecurity build version

**Syntax:** `MODSEC_BUILD`

**Implementation Status:** No

WGE can parse this directive, but the functionality is not implemented yet.

**Example:**

```apache
SecRule MODSEC_BUILD "@rx value" "id:1001,deny,msg:'Test'"
```
