+++
title = "DURATION"
weight = 92
+++

**Description:** Transaction processing duration

**Syntax:** `DURATION`

**Implementation Status:** No

WGE can parse this directive, but the functionality is not implemented yet.

**Example:**

```apache
SecRule DURATION "@rx value" "id:1001,deny,msg:'Test'"
```
