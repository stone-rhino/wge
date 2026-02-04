+++
title = "SecResponseBodyLimit"
weight = 9
+++

**Description:** Configure the maximum response body buffer size in bytes.

**Syntax:** `SecResponseBodyLimit LIMIT`

**Default:** 524288 (512KB)

Responses exceeding this limit will be rejected. This setting does not affect responses with MIME types that are not selected for buffering. The hard upper limit is 1 GB.

**Example:**

```
SecResponseBodyLimit 524288
```

**Case Sensitive:** Yes
