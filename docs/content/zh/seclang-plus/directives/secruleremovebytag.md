+++
title = "SecRuleRemoveByTag"
weight = 42
+++

**描述:** 根据标签移除规则。

            
**语法:** `SecRuleRemoveByTag "TAG"`

            
**区分大小写:** 是

            
            
通常使用 SecRuleRemoveById 删除规则，但有时通过 SecRuleRemoveByTag 禁用整组规则更为便捷。匹配采用区分大小写的字符串相等性。

            
**示例:**

            
```apache
SecRuleRemoveByTag "attack-sqli"
```
