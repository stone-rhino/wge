+++
title = "log"
weight = 18
+++

**描述:** 规则匹配成功后返回日志。

**语法:** `log`

**区分大小写:** 是

拥有这个 action 的规则在匹配成功后会调用日志回调接口返回日志，由服务器或者连接器写入日志。

**示例:**

```apache
SecRule ARGS "@rx test" "id:1001,log,status:403"
```