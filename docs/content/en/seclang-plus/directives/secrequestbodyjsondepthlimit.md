+++
title = "SecRequestBodyJsonDepthLimit"
weight = 6
+++

**Description:** Configure the maximum parsing depth for JSON objects.


**Syntax:** `SecRequestBodyJsonDepthLimit DEPTH`


**Default:** 0 (unlimited)


**Case Sensitive:** Yes

**Implemented:** No


When parsing JSON objects, if the nesting level exceeds the configured depth limit, parsing will terminate and set a REQBODY_ERROR error.

WGE can parse this directive correctly, but the functionality is not yet implemented.


**Example:**


```apache
SecRequestBodyJsonDepthLimit 512
```
