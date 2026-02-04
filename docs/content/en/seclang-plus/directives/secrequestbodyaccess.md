+++
title = "SecRequestBodyAccess"
weight = 2
+++

**Description:** Configure whether the WAF is allowed to access request body content.

**Syntax:** `SecRequestBodyAccess On|Off`

**Default:** Off

**Version:** 1.0

This directive must be enabled to inspect data transmitted in the request body (such as POST parameters). Request buffering is also essential for reliable blocking.

**Options:**

- **On:** Buffer and inspect the request body
- **Off:** Do not buffer the request body

**Example:**

```
SecRequestBodyAccess On
```

**Case Sensitive:** Yes
