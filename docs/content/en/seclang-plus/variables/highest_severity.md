+++
title = "HIGHEST_SEVERITY"
weight = 65
+++

**Description:** Highest severity level

**Syntax:** `HIGHEST_SEVERITY`

**Implementation Status:** No

WGE can parse this directive, but the functionality is not implemented yet.

**Example:**

```apache
SecRule HIGHEST_SEVERITY "@rx value" "id:1001,deny,msg:'Test'"
```
