+++
title = "SecRuleRemoveByMsg"
weight = 41
+++

**描述:** 根据msg内容移除规则。


**语法:** `SecRuleRemoveByMsg "STRING"`


**区分大小写:** 是



通常使用 SecRuleRemoveById 删除规则，但本指令支持通过匹配规则的 msg 操作进行删除。匹配采用区分大小写的字符串精确匹配。

与 SecRuleRemoveById 一致，其只能禁用在其之前的规则。

**示例:**


```apache
SecRuleRemoveByMsg "SQL Injection Attack"
```
