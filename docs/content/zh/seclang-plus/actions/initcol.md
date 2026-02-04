+++
title = "initcol"
weight = 31
+++

**描述:** 初始化持久化集合


**语法:** `initcol:collection=key`


initcol 动作用于初始化持久化集合（IP、SESSION、USER、RESOURCE、GLOBAL）。持久化集合允许在多个请求之间保存和共享数据，是实现 IP 频率限制、会话跟踪、用户行为分析等功能的基础。


**可用集合:**


- **IP** - 基于客户端 IP 地址的持久化存储
- **SESSION** - 基于会话 ID 的持久化存储
- **USER** - 基于用户 ID 的持久化存储
- **RESOURCE** - 基于资源标识的持久化存储
- **GLOBAL** - 全局持久化存储

集合数据会在规则执行过程中自动从存储加载，并在修改后自动持久化。


**示例:**


```apache
# 初始化 IP 集合用于频率限制
SecAction "id:1,phase:1,pass,nolog,initcol:IP=%{REMOTE_ADDR}"

# IP 频率限制实现
SecRule IP:request_count "@gt 100" \
    "id:2,phase:1,deny,msg:'IP 请求频率超限'"
SecRule REQUEST_URI "@unconditionalMatch" \
    "id:3,phase:1,pass,nolog,setvar:IP.request_count=+1,\
    expirevar:IP.request_count=60"

# 初始化会话集合
SecRule REQUEST_COOKIES:session_id "@rx ^[a-f0-9]{32}$" \
    "id:4,phase:1,pass,nolog,initcol:SESSION=%{REQUEST_COOKIES.session_id}"

# 初始化全局集合
SecAction "id:5,phase:1,pass,nolog,initcol:GLOBAL=global"
```


**参数类型:** `string`


**区分大小写:** 是
