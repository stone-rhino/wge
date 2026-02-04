+++
title = "t:trimRight"
weight = 38
+++

**描述:** 去除结尾空白


**语法:** `t:trimRight`


t:trimRight 去除输入字符串右侧（结尾）的所有空白字符，包括空格、制表符、换行符等。


**示例:**


```apache
# 去除参数结尾的空白后检测
SecRule ARGS:cmd "@rx ;$" \
    "id:1,phase:2,t:trimRight,deny,msg:'检测到命令分隔符'"

# 原始输入: "ls -la;   "
# 处理后: "ls -la;"
```


**参数类型:** `无`


**区分大小写:** 是
