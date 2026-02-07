+++
title = "maturity"
weight = 9
+++

**描述:** 成熟度级别 (1-9)

**语法:** `maturity:[1-9]`

**区分大小写:** 是

记录该规则的成熟级别，其中9是经过广泛测试，1是全新的实验性质的规则。

**示例:**

```apache
SecRule ARGS "@rx test" "id:1001,deny,maturity:9"
```