+++
title = "SecResponseBodyMimeType"
weight = 8
+++

**Description:** Configure which response body MIME types should be inspected.

**Syntax:** `SecResponseBodyMimeType MIME_TYPE1 MIME_TYPE2 ...`

**Case Sensitive:** Yes

**Implemented:** No

If you need this functionality, you can use content-type and skip/skipafter to control whether rules are executed for specific MIME types.

Alternatively, you can read the configuration parsed by WGE in the server and use it to determine whether to pass the body.

When this directive appears multiple times, the final parsed result is a merge of all directives.

**Example:**

```apache
SecResponseBodyMimeType text/plain text/html text/xml
```
