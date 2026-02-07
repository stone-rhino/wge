+++
title = "msg"
weight = 3
+++

**描述:** 日志消息

**语法:** `msg:'value'`

**区分大小写:** 是

与 logdata 类似，其同样也支持宏扩展功能。

**示例:**

```apache
SecRule ARGS "@rx test" "id:1001,deny,msg:'SQL Injection Detected'"

# 使用宏扩招的msg
SecRule ARGS "@rx test2" "id:1002,deny,msg:'Deny the ip %{REMOTE_ADDR}'"

```