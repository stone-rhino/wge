+++
title = "ARGS_COMBINED_SIZE"
weight = 7
+++

**Description:** Combined size of all parameters

**Syntax:** `ARGS_COMBINED_SIZE`

**Implementation Status:** No

WGE can parse this directive, but the functionality is not implemented yet.

**Example:**

```apache
SecRule ARGS_COMBINED_SIZE "@eq 10" "id:1001,deny,msg:'Test'"
```
