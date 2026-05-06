+++
title = "SecRuleUpdateOperatorById"
weight = 62
+++

**描述:** 通过规则 ID 更新现有规则的操作符。


**语法:** `SecRuleUpdateOperatorById ID [ID...] "OPERATOR"`


**区分大小写:** 是

**WGE 扩展:** 这是 WGE 特有的扩展，在标准 ModSecurity 中不可用。


此指令允许您通过 ID 修改一个或多个现有规则的操作符。这对于自定义第三方规则集（如 OWASP CRS）非常有用，无需修改原始规则文件。

ID 可以是单个规则 ID、范围（如 `100-200`）或带链索引的 ID（如 `100:1` 表示第一个链式规则）。


**示例:**


```apache
# 将规则 942100 更改为使用不区分大小写的匹配
SecRuleUpdateOperatorById 942100 "@rx (?i)select.*from"

# 同时更新多个规则
SecRuleUpdateOperatorById 942100 942110 942120 "@pm select insert update delete"

# 更新规则范围
SecRuleUpdateOperatorById 942100-942199 "@rx new_pattern"
```
