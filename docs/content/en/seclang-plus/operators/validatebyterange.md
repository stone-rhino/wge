+++
title = "@validateByteRange"
weight = 21
+++

**Description:** Validate included bytes

**Syntax:** `@validateByteRange range1,range2-range3,...`

**Input Data Type:** `string`

**Case Sensitive:** Yes

`@validateByteRange` checks whether each byte of the input falls within the specified range set. It supports single values (for example `10`) and ranges (for example `32-126`) separated by commas. It matches when bytes outside the allowed ranges are found. It is commonly used to detect NUL bytes (0x00), non-printable characters, and binary-like payloads.

**Note:**
- Parameters are parsed as positive integers using prefix parsing. For example, `10ab` is parsed as `10`; invalid forms such as `-100` are parsed as `0`.
- Values greater than `256` are dropped.
- If range start is greater than range end (for example `100-20`), that range is dropped.

**Example:**

```apache
# Detect NUL bytes (common evasion technique)
SecRule ARGS "@validateByteRange 1-255" \
    "id:1109,phase:2,deny,msg:'NUL byte injection detected'"

# Only allow printable ASCII characters and common control characters
SecRule ARGS "@validateByteRange 9,10,13,32-126" \
    "id:1110,phase:2,deny,msg:'Illegal characters detected'"
```
