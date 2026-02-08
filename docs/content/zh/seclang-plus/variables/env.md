+++
title = "ENV"
weight = 67
+++

**描述:** 环境变量集合


**语法:** `ENV`


**是否实现:** 否

**示例:**


```apache
SecRule ENV "@rx value" "id:1001,deny,msg:'Test'"
```
