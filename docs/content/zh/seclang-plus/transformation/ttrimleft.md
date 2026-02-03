+++
title = "t:trimLeft"
weight = 37
+++

**描述:** 去除开头空白


**语法:** `t:trimLeft`


t:trimLeft 去除输入字符串左侧（开头）的所有空白字符，包括空格、制表符、换行符等。


**示例:**


```apache
# 去除参数开头的空白后检测
SecRule ARGS:username "@rx ^admin" \
    "id:1,phase:2,t:trimLeft,deny,msg:'检测到 admin 用户名'"

# 原始输入: "   admin"
# 处理后: "admin"
```


**参数类型:** `无`


**区分大小写:** 是
