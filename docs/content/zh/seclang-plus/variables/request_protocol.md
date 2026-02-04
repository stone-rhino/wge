+++
title = "REQUEST_PROTOCOL"
weight = 19
+++

**描述:** 请求协议 (HTTP/1.1 等)


**语法:** `REQUEST_PROTOCOL`


REQUEST_PROTOCOL 包含请求使用的 HTTP 协议版本，如 HTTP/1.0、HTTP/1.1 或 HTTP/2.0。可用于检测协议违规或强制使用特定协议版本。


**示例:**


```apache
# 拒绝 HTTP/1.0 请求
SecRule REQUEST_PROTOCOL "@streq HTTP/1.0" \
    "id:1028,phase:1,deny,msg:'不支持 HTTP/1.0'"

# 检测异常协议
SecRule REQUEST_PROTOCOL "!@rx ^HTTP/[12]\.[0-9]$" \
    "id:1029,phase:1,deny,msg:'无效的 HTTP 协议'"
```
