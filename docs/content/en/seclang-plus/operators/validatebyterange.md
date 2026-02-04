+++
title = "@validateByteRange"
weight = 21
+++

**Description:** Validate byte range

**Syntax:** `"@validateByteRange range1,range2-range3,..."`

@validateByteRange checks if each byte in the variable value is within the specified range. Supports combinations of single values (e.g., 10) and ranges (e.g., 32-126), separated by commas. Matches if bytes outside the specified range are found. This operator is most commonly used for detecting the presence of NUL bytes (0x00) — these bytes have no legitimate purpose but are often used as detection evasion techniques. Can also be used to detect non-printable characters and binary data.

**Example:**

```apache
# Detect NUL bytes (common evasion technique)
SecRule ARGS "@validateByteRange 1-255" \
    "id:1109,phase:2,deny,msg:'NUL byte injection detected'"

# Only allow printable ASCII characters (32-126) and common control characters
SecRule ARGS "@validateByteRange 9,10,13,32-126" \
    "id:1110,phase:2,deny,msg:'Illegal characters detected'"

# Detect binary data in parameters
SecRule ARGS:data "!@validateByteRange 32-126" \
    "id:1111,phase:2,pass,log,msg:'Parameter contains non-printable characters'"

# Detect illegal characters in URL
SecRule REQUEST_URI "@validateByteRange 1-255" \
    "id:1112,phase:1,deny,msg:'NUL byte detected in URL'"
```

**Parameter Type:** `string`

**Case Sensitive:** Yes
