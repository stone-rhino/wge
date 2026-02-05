+++
title = "SecRequestBodyAccess"
weight = 2
+++

**Description:** Configure whether the WAF is allowed to access request body content.


**Syntax:** `SecRequestBodyAccess On|Off`


**Default:** Off


**Case Sensitive:** Yes


This directive must be enabled to inspect data transmitted in the request body (such as POST parameters). Request buffering is also essential for reliable blocking. Options:
- **On:** Buffer and inspect the request body
- **Off:** Do not buffer the request body

This directive does not actually affect whether WGE buffers the request body, because WGE cannot control the buffering action itself - it can only passively receive the request body from the server. For this directive to take effect, related logic needs to be added in the server.


**Example:**


```apache
SecRequestBodyAccess On
```
