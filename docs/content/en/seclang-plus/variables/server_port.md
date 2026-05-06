+++
title = "SERVER_PORT"
weight = 37
+++

**Description:** Server port

**Syntax:** `SERVER_PORT`

**Implementation Status:** No

WGE can parse this directive, but the functionality is not implemented yet.

**Example:**

```apache
SecRule SERVER_PORT "@rx value" "id:1001,deny,msg:'Test'"
```
