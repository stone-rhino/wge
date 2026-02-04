+++
title = "SecResponseBodyLimitAction"
weight = 24
+++

**Description:** Configure how to handle responses when the body exceeds the SecResponseBodyLimit.

**Syntax:** `SecResponseBodyLimitAction Reject|ProcessPartial`

**Default:** Reject

**Options:**

- **Reject:** Reject responses that exceed the limit
- **ProcessPartial:** Only inspect the portion of the response body that fits within the limit, pass through the rest

Some sites generate very long responses, making it difficult to set a reasonable limit. By setting this to ProcessPartial, you can handle oversized responses without significantly increasing the memory limit, but there is a risk that some content will not be inspected.

**Example:**

```
SecResponseBodyLimitAction ProcessPartial
```

**Case Sensitive:** Yes
