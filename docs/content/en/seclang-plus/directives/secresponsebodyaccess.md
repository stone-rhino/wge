+++
title = "SecResponseBodyAccess"
weight = 7
+++

**Description:** Configure whether the WAF is allowed to access response body content.

**Syntax:** `SecResponseBodyAccess On|Off`

**Default:** Off

This directive must be enabled to inspect HTML responses and implement response blocking.

**Options:**

- **On:** Buffer and inspect the response body
- **Off:** Do not buffer the response body

**Example:**

```
SecResponseBodyAccess On
```

**Case Sensitive:** Yes
