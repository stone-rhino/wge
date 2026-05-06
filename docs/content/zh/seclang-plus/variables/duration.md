+++
title = "DURATION"
weight = 92
+++

**描述:** 事务处理时长


**语法:** `DURATION`


**是否实现:** 否

此指令WGE可以正常解析，但是暂未实现该功能。

**示例:**

```apache
SecRule DURATION "@rx value" "id:1001,deny,msg:'Test'"
```
