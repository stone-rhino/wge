+++
title = "t:urlDecode"
weight = 4
+++

**Description:** URL decode

**Syntax:** `t:urlDecode`

Decodes a URL-encoded input string. Invalid encodings (those using non-hexadecimal characters, or incomplete encodings at the end of the string missing one or two bytes) will not be converted but will not throw an error. To detect invalid encodings, first use the @validateUrlEncoding operator on the input data. Do not use this transformation on variables that have already been URL-decoded (such as request parameters), unless you intentionally want to perform double URL decoding!

**Example:**

```apache
SecRule ARGS "@rx test" "id:1001,t:urlDecode"
```

**Parameter Type:** `string`
