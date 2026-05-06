+++
title = "REQUEST_BODY_LENGTH"
weight = 13
+++

**Description:** Request body length

**Syntax:** `REQUEST_BODY_LENGTH`

**Implementation Status:** No

WGE can parse this directive, but the functionality is not implemented yet.

**Example:**

```apache
SecRule REQUEST_BODY_LENGTH "@rx value" "id:1001,deny,msg:'Test'"
```
