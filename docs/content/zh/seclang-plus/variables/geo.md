+++
title = "GEO"
weight = 68
+++

**描述:** 地理位置信息

**语法:** `GEO`

**是否实现:** 否

此指令WGE可以正常解析，但是暂未实现该功能。


**示例:**

```apache
SecRule GEO "@rx value" "id:1001,deny,msg:'Test'"
```
