+++
title = "redirect"
weight = 15
+++

**描述:** 重定向到指定 URL

**语法:** `redirect:url`

**区分大小写:** 是

**是否实现:** 否

此指令并未在WGE中实现，无法使用。

**示例:**

```apache
SecRule ARGS "@rx test" "id:1001,redirect,status:403"
```