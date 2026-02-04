+++
title = "REQUEST_HEADERS"
weight = 8
+++

**描述:** 请求头


**语法:** `REQUEST_HEADERS | REQUEST_HEADERS:HeaderName`


REQUEST_HEADERS 包含所有 HTTP 请求头。可以使用 REQUEST_HEADERS:HeaderName 访问特定头部，如 User-Agent、Host、Content-Type 等。请求头是攻击者常用的注入点，应对关键头部进行检测。


**示例:**


```apache
# 检查 User-Agent 中的恶意模式
SecRule REQUEST_HEADERS:User-Agent "@pm nikto sqlmap nmap" \
    "id:1009,phase:1,deny,msg:'检测到扫描工具 User-Agent'"

# 验证 Host 头格式
SecRule REQUEST_HEADERS:Host "!@rx ^[a-zA-Z0-9\.\-]+$" \
    "id:1010,phase:1,deny,msg:'无效的 Host 头'"

# 检查 Referer 头中的 XSS
SecRule REQUEST_HEADERS:Referer "@detectXSS" \
    "id:1011,phase:1,deny,msg:'Referer 头检测到 XSS'"
```
