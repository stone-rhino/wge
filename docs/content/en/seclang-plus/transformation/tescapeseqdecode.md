+++
title = "t:escapeSeqDecode"
weight = 16
+++

**Description:** Escape sequence decode

**Syntax:** `t:escapeSeqDecode`

Decodes ANSI C escape sequences: \a, \b, \f, \n, \r, \t, \v, \\, \?, \', \", \xHH (hexadecimal), and \0OOO (octal). Invalid encodings are preserved in the output unchanged.

**Example:**

```apache
SecRule ARGS "@rx test" "id:1001,t:escapeSeqDecode"
```

**Parameter Type:** `string`
