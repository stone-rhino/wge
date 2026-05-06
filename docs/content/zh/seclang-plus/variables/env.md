+++
title = "ENV"
weight = 67
+++

**描述:** 环境变量集合


**语法:** `ENV`

**是否实现:** 否

此指令WGE可以正常解析，但是暂未实现该功能。

**示例:**

```apache
SecRule ENV "@rx value" "id:1001,deny,msg:'Test'"
```
