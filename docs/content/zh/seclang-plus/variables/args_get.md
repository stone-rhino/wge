+++
title = "ARGS_GET"
weight = 2
+++

**描述:** GET 请求参数


**语法:** `ARGS_GET | ARGS_GET:Key`


ARGS_GET 仅包含 URL 查询字符串中传递的参数（即 GET 参数）。与 ARGS 不同，它不包含 POST 请求体中的参数，因此可用于仅检查 URL 传递的数据，避免误检 POST 数据。可以通过 ARGS_GET:paramname 访问特定参数。


**示例:**


```apache
# 检查 URL 查询参数中的 SQL 注入
SecRule ARGS_GET "@detectSQLi" "id:1001,phase:1,deny,status:403,msg:'GET 参数检测到 SQL 注入'"

# 检查特定的 GET 参数
SecRule ARGS_GET:id "@rx [^0-9]" "id:1002,phase:1,deny,msg:'ID 参数必须为数字'"
```
