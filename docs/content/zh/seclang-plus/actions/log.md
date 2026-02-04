+++
title = "log"
weight = 18
+++

**描述:** 记录到错误日志

**信息:** Original Example: `log`

**语法:** `log | log:value`

**示例:**

```apache
SecRule ARGS "@rx test" "id:1001,log,status:403"
```


**参数类型:** `无`


**区分大小写:** 是
