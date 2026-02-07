+++
title = "rev"
weight = 7
+++

**描述:** 规则修订版本

**语法:** `rev:'string'`

**参数类型:** `string`

**区分大小写:** 是

设置该规则的目前修订的版本。

**示例:**

```apache
SecRule ARGS "@rx test" "id:1001,deny,rev:'version 2.1.0'"
```