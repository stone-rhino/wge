+++
title = "ARGS_GET_NAMES"
weight = 5
+++

**描述:** GET 参数名称


**语法:** `ARGS_GET_NAMES`


ARGS_GET_NAMES 仅包含 URL 查询字符串中参数的名称。用于检测 GET 参数名是否包含可疑内容，而不检查 POST 参数名。


**示例:**


```apache
SecRule ARGS_GET_NAMES "@detectSQLi" "id:1007,phase:1,deny,msg:'GET 参数名检测到 SQL 注入'"
```
