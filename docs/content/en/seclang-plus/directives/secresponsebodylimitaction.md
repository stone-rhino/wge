+++
title = "SecResponseBodyLimitAction"
weight = 24
+++

**Description:** Configure how to handle responses when the body exceeds the SecResponseBodyLimit.


**Syntax:** `SecResponseBodyLimitAction Reject|ProcessPartial`


**Default:** ProcessPartial


**Options:**


- **Reject:** Reject responses that exceed the limit
- **ProcessPartial:** Only inspect the portion of the response body that fits within the limit, pass through the rest

**Case Sensitive:** Yes

**Implemented:** No


WGE can parse this directive correctly, but the functionality is not yet implemented.


**Example:**


```apache
SecResponseBodyLimitAction ProcessPartial
```
