+++
title = "nolog"
weight = 19
+++

**描述:** 不记录日志。

**语法:** `nolog`

**区分大小写:** 是

与动作 log 相反，动作 nolog 标明该规则不需要日志，所以即使匹配成功也不会调用日志回调函数。

**示例:**

```apache
SecRule ARGS "@rx test" "id:1001,nolog,status:403"
```