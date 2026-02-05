+++
title = "SecUploadKeepFiles"
weight = 36
+++

**Description:** Configure whether to keep uploaded files after request processing is complete.

**Syntax:** `SecUploadKeepFiles On|Off`

**Default:** Off

**Options:**

- **On:** Keep all uploaded files
- **Off:** Do not keep uploaded files

**Case Sensitive:** Yes

**Implemented:** No

This directive requires that the storage directory has been defined via SecUploadDir.

WGE can parse this directive correctly, but the functionality is not yet implemented.

**Example:**

```apache
SecUploadKeepFiles RelevantOnly
```
