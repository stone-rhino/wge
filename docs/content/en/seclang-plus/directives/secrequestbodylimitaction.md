+++
title = "SecRequestBodyLimitAction"
weight = 5
+++

**Description:** Configure how to handle requests when the body exceeds the SecRequestBodyLimit.

**Syntax:** `SecRequestBodyLimitAction Reject|ProcessPartial`

**Default:** Reject

**Options:**

- **Reject:** Reject requests that exceed the limit
- **ProcessPartial:** Only inspect the portion of the request body that fits within the limit, pass through the rest

When the WAF is in DetectionOnly mode and needs to run completely passively, using ProcessPartial avoids interfering with transactions. You can detect whether the request body exceeded the limit by creating a rule that checks the INBOUND_DATA_ERROR variable.

**Example:**

```
SecRequestBodyLimitAction Reject
```

**Case Sensitive:** Yes
