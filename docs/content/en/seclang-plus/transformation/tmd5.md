+++
title = "t:md5"
weight = 34
+++

**Description:** Calculate MD5 hash

**Syntax:** `t:md5`

Calculates the MD5 hash of the input string. The result is in raw binary format and may need to be converted to text for printing or logging. Often combined with hexadecimal encoding (e.g., t:md5,t:hexEncode).

**Example:**

```apache
SecRule ARGS "@rx test" "id:1001,t:md5,t:hexEncode"
```

**Parameter Type:** `string`

**Implementation Status:** Not yet implemented
