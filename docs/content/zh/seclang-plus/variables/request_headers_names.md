+++
title = "REQUEST_HEADERS_NAMES"
weight = 9
+++

**描述:** 请求头名称


**语法:** `REQUEST_HEADERS_NAMES`


REQUEST_HEADERS_NAMES 包含所有请求头的名称（不包括值）。可用于检测是否存在异常的自定义头部，或检测头部名称本身是否包含攻击载荷。


**示例:**


```apache
# 检测头部名称中的异常字符
SecRule REQUEST_HEADERS_NAMES "@rx [<>'\"]" \
    "id:1012,phase:1,deny,msg:'请求头名称包含非法字符'"
```
