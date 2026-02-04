+++
title = "SecRequestBodyNoFilesLimit"
weight = 4
+++

**Description:** Configure the maximum request body size in bytes for requests without file uploads.

**Syntax:** `SecRequestBodyNoFilesLimit LIMIT`

**Default:** 131072 (128KB)

Generally speaking, the default value is not small enough. For most applications, you should be able to reduce it to 128 KB or lower. Requests exceeding this limit will be rejected with a 413 (Request Entity Too Large) status code. The hard upper limit is 1 GB.

**Example:**

```
SecRequestBodyNoFilesLimit 131072
```

**Case Sensitive:** Yes
