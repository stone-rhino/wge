+++
title = "status"
weight = 44
+++

**描述:** 设置 HTTP 响应状态码

**语法:** `status:CODE`

status 动作设置当破坏性动作（deny、block、drop）被触发时返回的 HTTP 响应状态码。这允许对错误响应进行精细控制。

常用状态码：
- **400** - Bad Request（格式错误的输入）
- **401** - Unauthorized（需要认证）
- **403** - Forbidden（访问被拒绝）
- **404** - Not Found（隐藏资源存在）
- **405** - Method Not Allowed（方法不允许）
- **429** - Too Many Requests（请求频率限制）
- **500** - Internal Server Error（内部服务器错误）
- **503** - Service Unavailable（服务不可用）

**示例:**

```apache
# 对 SQL 注入尝试返回 403 Forbidden
SecRule ARGS "@detectSQLi" \
    "id:100,phase:2,deny,status:403,msg:'SQL 注入已阻止'"

# 对缺少认证返回 401
SecRule REQUEST_HEADERS:Authorization "@eq ''" \
    "id:101,phase:1,deny,status:401,msg:'需要认证'"

# 对超出速率限制返回 429
SecRule IP:request_count "@gt 100" \
    "id:102,phase:1,deny,status:429,msg:'超出速率限制'"

# 返回 404 以隐藏管理路径
SecRule REQUEST_URI "^/admin" "chain,id:103,phase:1,deny,status:404"
    SecRule REMOTE_ADDR "!@ipMatch 10.0.0.0/8" ""
```

**参数类型:** `integer`

**区分大小写:** 是
