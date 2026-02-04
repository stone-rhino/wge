+++
title = "ARGS_POST"
weight = 3
+++

**描述:** POST 请求参数


**语法:** `ARGS_POST | ARGS_POST:Key`


ARGS_POST 仅包含请求体中传递的参数（即 POST 数据）。WGE 支持解析 application/x-www-form-urlencoded、multipart/form-data、application/json 和 application/xml 格式的请求体。使用此变量可以仅检查表单提交的数据，而不检查 URL 参数。需要启用 SecRequestBodyAccess 才能访问此变量。


**示例:**


```apache
# 检查 POST 表单参数中的 XSS
SecRule ARGS_POST "@detectXSS" "id:1003,phase:2,deny,status:403,msg:'POST 参数检测到 XSS'"

# 验证登录表单的用户名格式
SecRule ARGS_POST:username "!@rx ^[a-zA-Z0-9_]{3,20}$" \
    "id:1004,phase:2,deny,msg:'用户名格式无效'"
```
