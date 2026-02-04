+++
title = "SecUnicodeMapFile"
weight = 16
+++

**Description:** Configure the Unicode mapping file path and code page.

**Syntax:** `SecUnicodeMapFile PATH CODEPAGE`

This directive defines the Unicode mapping file path used by the urlDecodeUni transformation function. The code page parameter specifies the target character encoding (e.g., 20127 represents US-ASCII). When processing URLs that use non-ASCII encoding, this configuration ensures correct character conversion.

**Example:**

```
SecUnicodeMapFile unicode.mapping 20127
```

**Case Sensitive:** Yes
