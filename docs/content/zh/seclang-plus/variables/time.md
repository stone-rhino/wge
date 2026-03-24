+++
title = "TIME"
weight = 93
+++

**描述:** 当前时间 (时:分:秒格式字符串)

**语法:** `TIME`

**是否实现:** 否

此指令WGE可以正常解析，但是暂未实现该功能。

**示例:**

```apache
SecRule TIME "@rx value" "id:1001,deny,msg:'Test'"
```
