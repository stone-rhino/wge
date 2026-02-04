+++
title = "@verifyCc"
weight = 26
+++

**Description:** Verify credit card number

**Syntax:** `"@verifyCc parameter"`

**Example:**

```apache
SecRule ARGS "@verifyCc parameter" "id:1001,deny,msg:'Test'"
```

**Parameter Type:** `string (regex)`

**Implementation Status:** Not yet implemented
