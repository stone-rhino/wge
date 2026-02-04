+++
title = "msg"
weight = 3
+++

**描述:** 日志消息

**语法:** `msg:'message'`

**示例:**

```apache
SecRule ARGS "@rx test" "id:1001,deny,msg:'SQL Injection Detected'"
```


**参数类型:** `string`


**区分大小写:** 是
