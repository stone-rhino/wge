+++
title = "REQUEST_COOKIES_NAMES"
weight = 11
+++

**描述:** Cookie 名称


**语法:** `REQUEST_COOKIES_NAMES`


REQUEST_COOKIES_NAMES 包含所有 Cookie 的名称。可用于检测 Cookie 名称是否包含异常字符或攻击载荷。


**示例:**


```apache
# 检测 Cookie 名称中的异常字符
SecRule REQUEST_COOKIES_NAMES "@rx [<>'\";()]" \
    "id:1015,phase:1,deny,msg:'Cookie 名称包含非法字符'"
```
