+++
title = "maturity"
weight = 9
+++

**描述:** 成熟度级别 (1-9)

**语法:** `maturity:level`

**示例:**

```apache
SecRule ARGS "@rx test" "id:1001,deny,maturity:9"
```


**参数类型:** `int`


**区分大小写:** 是
