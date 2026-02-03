+++
title = "t:jsDecode"
weight = 14
+++

**Description:** JavaScript decode

**Syntax:** `t:jsDecode`

Decodes JavaScript escape sequences. For \uHHHH codes in the range FF01-FF5E (fullwidth ASCII), the high byte is used to detect and adjust the low byte; otherwise only the low byte is used and the high byte is zeroed (which may result in information loss).

**Example:**

```apache
SecRule ARGS "@rx test" "id:1001,t:jsDecode"
```

**Parameter Type:** `string`
