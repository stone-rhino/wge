+++
title = "SecResponseBodyMimeTypesClear"
weight = 23
+++

**Description:** Clear all response body MIME type configurations.


**Syntax:** `SecResponseBodyMimeTypesClear`


**Case Sensitive:** Yes



This directive clears all MIME types previously configured through SecResponseBodyMimeType, allowing you to redefine which response types need to be buffered from scratch. Typically used to override inherited configurations.


**Example:**


```apache
SecResponseBodyMimeTypesClear
SecResponseBodyMimeType text/html text/plain
```
