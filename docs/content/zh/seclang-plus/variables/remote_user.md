+++
title = "REMOTE_USER"
weight = 34
+++

**描述:** 认证用户名


**语法:** `REMOTE_USER`


**是否实现:** 否

**示例:**


```apache
SecRule REMOTE_USER "@rx value" "id:1001,deny,msg:'Test'"
```
