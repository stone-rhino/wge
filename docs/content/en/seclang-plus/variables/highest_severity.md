+++
title = "HIGHEST_SEVERITY"
weight = 65
+++

**Description:** Highest severity level

**Syntax:** `HIGHEST_SEVERITY`

**Implementation Status:** No

**Example:**

```apache
SecRule HIGHEST_SEVERITY "@rx value" "id:1001,deny,msg:'Test'"
```
