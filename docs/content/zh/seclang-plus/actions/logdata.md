+++
title = "logdata"
weight = 22
+++

**描述:** 记录指定数据到日志。

**语法:** `logdata:'value'`

**区分大小写:** 是

设置规则匹配成功后的上报的日志，其中可以使用宏扩展功能。

**示例:**

```apache
SecRule ARGS "@rx test" "id:1001,logdata:'the deny ip is %{REMOTE_ADDR}',status:403"
```