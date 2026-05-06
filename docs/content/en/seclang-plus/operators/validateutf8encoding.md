+++
title = "@validateUtf8Encoding"
weight = 23
+++

**Description:** Validate UTF-8 encoding

**Syntax:** `"@validateUtf8Encoding parameter"`

**Example:**

```apache
SecRule ARGS "@validateUtf8Encoding parameter" "id:1001,deny,msg:'Test'"
```

**Parameter Type:** `none`

**Implementation Status:** Not yet implemented
