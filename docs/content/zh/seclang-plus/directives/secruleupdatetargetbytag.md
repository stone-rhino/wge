+++
title = "SecRuleUpdateTargetByTag"
weight = 46
+++

**描述:** 根据标签更新规则的参数列表。


**语法:** `SecRuleUpdateTargetByTag "TAG" VARIABLES`


**区分大小写:** 是

与 SecRuleUpdateTargetById SecRuleUpdateTargetByTag 是通过tag修改指定规则的变量，更多注意事项可见 SecRuleUpdateTargetById。


**示例:**


```apache
SecRuleUpdateTargetByTag "attack-sqli" !ARGS:search
```
