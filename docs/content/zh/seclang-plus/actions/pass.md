+++
title = "pass"
weight = 16
+++

**描述:** 匹配成功后仍旧进行后续规则的处理。

**语法:** `pass`

**区分大小写:** 是

**示例:**

```apache
SecRule ARGS "@rx test" "id:1001,pass,status:403"
```