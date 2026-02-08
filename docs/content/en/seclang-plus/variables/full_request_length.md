+++
title = "FULL_REQUEST_LENGTH"
weight = 23
+++

**Description:** Complete request length

**Syntax:** `FULL_REQUEST_LENGTH`

**Implementation Status:** No

**Example:**

```apache
SecRule FULL_REQUEST_LENGTH "@rx value" "id:1001,deny,msg:'Test'"
```
