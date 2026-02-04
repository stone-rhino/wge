+++
title = "ver"
weight = 6
+++

**描述:** 规则版本

**语法:** `ver:'version'`

**示例:**

```apache
SecRule ARGS "@rx test" "id:1001,deny,ver:'1.0.0'"
```


**参数类型:** `string`


**区分大小写:** 是
