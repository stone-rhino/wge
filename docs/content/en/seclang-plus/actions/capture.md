+++
title = "capture"
weight = 23
+++

**Description:** Capture regex match content

**Syntax:** `capture | capture:value`

The capture action captures the content matched by a regular expression operator. The captured groups are stored in TX:0 through TX:9 variables, where TX:0 contains the full match and TX:1 through TX:9 contain the captured subgroups. This is useful for extracting and reusing matched data in subsequent rules or log messages.

**Example:**

```apache
SecRule ARGS "@rx test" "id:1001,capture,status:403"
```

**Parameter Type:** `none`

**Case Sensitive:** Yes
