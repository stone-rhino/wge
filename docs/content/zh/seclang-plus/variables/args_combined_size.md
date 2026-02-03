+++
title = "ARGS_COMBINED_SIZE"
weight = 7
+++

**描述:** 所有参数的总大小


**语法:** `ARGS_COMBINED_SIZE | ARGS_COMBINED_SIZE:Key`


**示例:**


```apache
SecRule ARGS_COMBINED_SIZE:User-Agent "@rx box" "id:1001,deny,msg:'Test'"
```


**是否实现:** 暂无
