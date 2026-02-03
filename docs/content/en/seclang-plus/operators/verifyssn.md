+++
title = "@verifySsn"
weight = 28
+++

**Description:** Verify US Social Security Number

**Syntax:** `"@verifySsn parameter"`

**Example:**

```apache
SecRule ARGS "@verifySsn parameter" "id:1001,deny,msg:'Test'"
```

**Parameter Type:** `string (regex)`

**Implementation Status:** Not yet implemented
