+++
title = "REQUEST_BODY_LENGTH"
weight = 13
+++

**Description:** Request body length

**Syntax:** `REQUEST_BODY_LENGTH`

**Example:**

```apache
SecRule REQUEST_BODY_LENGTH "@rx value" "id:1001,deny,msg:'Test'"
```

**Implementation Status:** Not yet implemented
