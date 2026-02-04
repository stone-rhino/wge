+++
title = "SecUploadKeepFiles"
weight = 36
+++

**Description:** Configure whether to keep uploaded files after request processing is complete.

**Syntax:** `SecUploadKeepFiles On|Off|RelevantOnly`

**Default:** Off

**Options:**

- **On:** Keep all uploaded files
- **Off:** Do not keep uploaded files
- **RelevantOnly:** Only keep uploaded files from requests that triggered rules

**Case Sensitive:** Yes

This directive requires that the storage directory has been defined via SecUploadDir.

**Example:**

```
SecUploadKeepFiles RelevantOnly
```
