+++
title = "t:sha1"
weight = 35
+++

**Description:** Calculate SHA1 hash

**Syntax:** `t:sha1`

Calculates the SHA1 hash of the input string. The result is in raw binary format and may need to be converted to text for printing or logging. Hash functions are often combined with hexadecimal encoding (e.g., t:sha1,t:hexEncode).

**Example:**

```apache
SecRule ARGS "@rx test" "id:1001,t:sha1,t:hexEncode"
```

**Parameter Type:** `string`
