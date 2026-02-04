+++
title = "SecRequestBodyJsonDepthLimit"
weight = 6
+++

**Description:** Configure the maximum parsing depth for JSON objects.

**Syntax:** `SecRequestBodyJsonDepthLimit DEPTH`

**Default:** 512

When parsing JSON objects, if the nesting level exceeds the configured depth limit, parsing will terminate and set a REQBODY_ERROR error.

**Example:**

```
SecRequestBodyJsonDepthLimit 512
```

**Case Sensitive:** Yes
