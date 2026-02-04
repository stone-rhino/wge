+++
title = "t:urlEncode"
weight = 6
+++

**Description:** URL encode

**Syntax:** `t:urlEncode`

Encodes the input string using URL encoding (percent-encoding). Special characters are converted to their %XX hexadecimal representation.

**Example:**

```apache
SecRule ARGS "@rx test" "id:1001,t:urlEncode"
```

**Parameter Type:** `string`

**Implementation Status:** Not yet implemented
