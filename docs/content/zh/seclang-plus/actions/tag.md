+++
title = "tag"
weight = 5
+++

**描述:** 为规则指定标签。

**语法:** `tag:'string'`

**区分大小写:** 是

这个标签可以被 SecRuleRemoveByTag 等使用，如果需要大批量一次操作多个规则，可以用动作 tag 表示它们。

**示例:**

```apache
SecRule ARGS "@rx test" "id:1001,deny,tag:'attack-sqli'"
```