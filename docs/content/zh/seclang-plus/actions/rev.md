+++
title = "rev"
weight = 7
+++

**描述:** 规则修订版本

**语法:** `rev:'revision'`

**示例:**

```apache
SecRule ARGS "@rx test" "id:1001,deny,rev:'2'"
```


**参数类型:** `string`


**区分大小写:** 是
