+++
title = "REQUEST_METHOD"
weight = 14
+++

**描述:** 请求方法 (GET, POST 等)


**语法:** `REQUEST_METHOD`


REQUEST_METHOD 包含 HTTP 请求方法，如 GET、POST、PUT、DELETE、HEAD、OPTIONS 等。可用于限制特定端点允许的请求方法，或检测异常的 HTTP 方法。


**示例:**


```apache
# 仅允许 GET 和 POST 方法
SecRule REQUEST_METHOD "!@pm GET POST HEAD" \
    "id:1018,phase:1,deny,msg:'不允许的 HTTP 方法'"

# 阻止 TRACE 方法（防止 XST 攻击）
SecRule REQUEST_METHOD "@streq TRACE" \
    "id:1019,phase:1,deny,msg:'TRACE 方法被禁止'"
```
