+++
title = "ver"
weight = 6
+++

**描述:** 指定规则集版本。

**语法:** `ver:'version'`

**区分大小写:** 是

设置规则集的版本，与 rev 不同的时 rev 标注的是单条规则的版本，而 ver 标明的是规则所在规则集的版本。

**示例:**

```apache
SecRule ARGS "@rx test" "id:1001,deny,ver:'1.0.0'"
```