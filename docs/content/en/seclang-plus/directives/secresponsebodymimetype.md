+++
title = "SecResponseBodyMimeType"
weight = 8
+++

**Description:** Configure which response body MIME types should be inspected.

**Syntax:** `SecResponseBodyMimeType MIME_TYPE1 MIME_TYPE2 ...`

You can use multiple SecResponseBodyMimeType directives to add MIME types. Use SecResponseBodyMimeTypesClear to clear previously configured MIME types and start fresh.

**Example:**

```
SecResponseBodyMimeType text/plain text/html text/xml
```

**Case Sensitive:** Yes
