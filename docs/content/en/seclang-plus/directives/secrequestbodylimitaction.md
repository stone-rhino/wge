+++
title = "SecRequestBodyLimitAction"
weight = 5
+++

**Description:** Configure how to handle requests when the body exceeds the SecRequestBodyLimit.

**Syntax:** `SecRequestBodyLimitAction Reject|ProcessPartial`

**Default:** ProcessPartial

**Options:**

- **Reject:** Reject requests that exceed the limit
- **ProcessPartial:** Only inspect the portion of the request body that fits within the limit, pass through the rest

**Case Sensitive:** Yes

**Implemented:** No

This means that even if the request body size exceeds the SecRequestBodyLimit setting, it will not affect rule execution or WGE's internal state.

**Example:**

```apache
SecRequestBodyLimitAction Reject
```
