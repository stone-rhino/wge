+++
title = "SecMarker"
weight = 60
+++

**Description:** Create a rule marker that can be used as a target for the `skipAfter` action.


**Syntax:** `SecMarker LABEL`


**Case Sensitive:** Yes


SecMarker creates a fixed-position rule marker that serves as a target for the `skipAfter` action. When `skipAfter` is triggered, rule processing skips forward to the marker with the matching label. This enables conditional rule execution by allowing groups of rules to be skipped.

The marker itself does not perform any action; it simply serves as a jump target.


**Example:**


```apache
SecRule REQUEST_URI "^/admin" "id:100,phase:1,pass,skipAfter:END_ADMIN_CHECK"
SecRule REQUEST_HEADERS:Authorization "^Basic" "id:101,phase:1,deny,status:401"
SecMarker END_ADMIN_CHECK
```

In this example, if the URI does not start with `/admin`, rule 101 is executed; if it does, rule processing skips directly to `END_ADMIN_CHECK`, bypassing rule 101.
