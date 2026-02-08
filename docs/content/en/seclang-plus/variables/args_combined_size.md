+++
title = "ARGS_COMBINED_SIZE"
weight = 7
+++

**Description:** Combined size of all parameters

**Syntax:** `ARGS_COMBINED_SIZE | ARGS_COMBINED_SIZE:Key`

**Implementation Status:** No

**Example:**

```apache
SecRule ARGS_COMBINED_SIZE:User-Agent "@rx box" "id:1001,deny,msg:'Test'"
```
