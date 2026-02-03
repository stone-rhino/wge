+++
title = "drop"
weight = 11
+++

**描述:** 丢弃连接

**信息:** Original Example: `drop`

**语法:** `drop | drop:value`

**示例:**

```apache
SecRule ARGS "@rx test" "id:1001,drop,status:403"
```


**参数类型:** `无`


**区分大小写:** 是
