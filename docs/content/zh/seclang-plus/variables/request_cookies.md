+++
title = "REQUEST_COOKIES"
weight = 10
+++

**描述:** 请求 Cookie


**语法:** `REQUEST_COOKIES | REQUEST_COOKIES:CookieName`


REQUEST_COOKIES 包含请求中所有 Cookie 的值。可以使用 REQUEST_COOKIES:name 访问特定 Cookie。Cookie 是常见的攻击载体，攻击者可能在 Cookie 中注入恶意代码或尝试会话劫持。


**示例:**


```apache
# 检查所有 Cookie 中的 SQL 注入
SecRule REQUEST_COOKIES "@detectSQLi" \
    "id:1013,phase:1,deny,msg:'Cookie 检测到 SQL 注入'"

# 验证会话 Cookie 格式
SecRule REQUEST_COOKIES:SESSIONID "!@rx ^[a-zA-Z0-9]{32}$" \
    "id:1014,phase:1,deny,msg:'无效的会话 Cookie 格式'"
```
