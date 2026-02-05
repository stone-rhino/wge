+++
title = "SecRuleRemoveByTag"
weight = 42
+++

**描述:** 根据tag移除规则。


**语法:** `SecRuleRemoveByTag "STRING"`


**区分大小写:** 是



通常使用 SecRuleRemoveById 删除规则，但有时通过 SecRuleRemoveByTag 禁用整组规则更为便捷，其中匹配是大小写区分的。

与 SecRuleRemoveById 一致，其只能禁用在其之前的规则。

**示例:**


```apache
SecRuleRemoveByTag "attack-sqli"
```
