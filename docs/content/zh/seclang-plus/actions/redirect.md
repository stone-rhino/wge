+++
title = "redirect"
weight = 15
+++

**描述:** 重定向到指定 URL

**信息:** Original Example: `redirect:https://example.com`

**语法:** `redirect | redirect:value`

**示例:**

```apache
SecRule ARGS "@rx test" "id:1001,redirect,status:403"
```


**参数类型:** `string`


**区分大小写:** 是
