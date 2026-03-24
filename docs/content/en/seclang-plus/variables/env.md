+++
title = "ENV"
weight = 67
+++

**Description:** Environment variable collection

**Syntax:** `ENV`

**Implementation Status:** No

WGE can parse this directive, but the functionality is not implemented yet.

**Example:**

```apache
SecRule ENV "@rx value" "id:1001,deny,msg:'Test'"
```
