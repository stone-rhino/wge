+++
title = "ARGS_NAMES"
weight = 4
+++

**描述:** 所有参数名称


**语法:** `ARGS_NAMES`


ARGS_NAMES 包含所有请求参数的名称（键名），包括 GET 和 POST 参数。此变量用于检测参数名称本身是否包含攻击载荷，这是一种常见的绕过技术，攻击者可能在参数名中注入恶意代码。


**示例:**


```apache
# 检测参数名中的 SQL 注入关键字
SecRule ARGS_NAMES "@pm select union insert delete drop" \
    "id:1005,phase:2,deny,msg:'参数名包含 SQL 关键字'"

# 限制参数名只能包含字母数字和下划线
SecRule ARGS_NAMES "!@rx ^[a-zA-Z0-9_\[\]\.]+$" \
    "id:1006,phase:2,deny,msg:'参数名包含非法字符'"
```
