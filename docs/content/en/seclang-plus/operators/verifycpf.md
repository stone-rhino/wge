+++
title = "@verifyCpf"
weight = 27
+++

**Description:** Verify Brazilian CPF number

**Syntax:** `"@verifyCpf parameter"`

**Example:**

```apache
SecRule ARGS "@verifyCpf parameter" "id:1001,deny,msg:'Test'"
```

**Parameter Type:** `string (regex)`

**Implementation Status:** Not yet implemented
