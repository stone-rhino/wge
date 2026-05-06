+++
title = "SecRuleRemoveById"
weight = 40
+++

**描述:** 根据 ID 移除规则。


**语法:** `SecRuleRemoveById [ID|ID_RANGE] ...`


**区分大小写:** 是



该指令支持多个参数，每个参数可为规则 ID 或范围。

**注意**：SecRuleRemoveById应该在最后加载，当SecRuleRemoveById出现在其想要禁用的规则之前时禁用不会生效。

**示例:**


```apache
# 移除单个规则
SecRuleRemoveById 1001

# 移除多个规则
SecRuleRemoveById 1001 1002 1003

# 移除范围内的规则
SecRuleRemoveById 1001-1100
```
