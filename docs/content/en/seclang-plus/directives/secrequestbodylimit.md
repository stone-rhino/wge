+++
title = "SecRequestBodyLimit"
weight = 3
+++

**Description:** Configure the maximum acceptable request body size in bytes.

**Syntax:** `SecRequestBodyLimit LIMIT`

**Default:** 13107200 (12.5MB)

Requests exceeding this limit will be rejected with a 413 (Request Entity Too Large) status code. The hard upper limit is 1 GB.

**Example:**

```
SecRequestBodyLimit 13107200
```

**Case Sensitive:** Yes
