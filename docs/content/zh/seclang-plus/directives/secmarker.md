+++
title = "SecMarker"
weight = 60
+++

**描述:** 创建一个规则标记，用作 `skipAfter` 动作的跳转目标。


**语法:** `SecMarker LABEL`


**区分大小写:** 是


SecMarker 创建一个固定位置的规则标记，作为 `skipAfter` 动作的跳转目标。当 `skipAfter` 被触发时，规则处理将跳转到具有匹配标签的标记处。这通过允许跳过规则组来实现条件性规则执行。

标记本身不执行任何动作，它仅作为跳转目标。


**示例:**


```apache
SecRule REQUEST_URI "^/admin" "id:100,phase:1,pass,skipAfter:END_ADMIN_CHECK"
SecRule REQUEST_HEADERS:Authorization "^Basic" "id:101,phase:1,deny,status:401"
SecMarker END_ADMIN_CHECK
```

在此示例中，如果 URI 不以 `/admin` 开头，则执行规则 101；如果是，则规则处理直接跳转到 `END_ADMIN_CHECK`，跳过规则 101。
