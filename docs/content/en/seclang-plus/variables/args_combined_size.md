+++
title = "ARGS_COMBINED_SIZE"
weight = 7
+++

**Description:** Combined size of all parameters

**Syntax:** `ARGS_COMBINED_SIZE | ARGS_COMBINED_SIZE:Key`

**Example:**

```apache
SecRule ARGS_COMBINED_SIZE:User-Agent "@rx box" "id:1001,deny,msg:'Test'"
```

**Implementation Status:** Not yet implemented
