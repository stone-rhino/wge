+++
title = "SERVER_NAME"
weight = 36
+++

**Description:** Server name

**Syntax:** `SERVER_NAME`

**Implementation Status:** No

WGE can parse this directive, but the functionality is not implemented yet.

**Example:**

```apache
SecRule SERVER_NAME "@rx value" "id:1001,deny,msg:'Test'"
```
