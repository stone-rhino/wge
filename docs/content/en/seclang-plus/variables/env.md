+++
title = "ENV"
weight = 67
+++

**Description:** Environment variable collection

**Syntax:** `ENV`

**Implementation Status:** No

**Example:**

```apache
SecRule ENV "@rx value" "id:1001,deny,msg:'Test'"
```
