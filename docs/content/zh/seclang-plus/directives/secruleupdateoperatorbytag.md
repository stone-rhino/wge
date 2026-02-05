+++
title = "SecRuleUpdateOperatorByTag"
weight = 63
+++

**描述:** 通过标签更新现有规则的操作符。


**语法:** `SecRuleUpdateOperatorByTag "TAG" "OPERATOR"`


**区分大小写:** 是

**WGE 扩展:** 这是 WGE 特有的扩展，在标准 ModSecurity 中不可用。


此指令允许您修改所有具有匹配标签的规则的操作符。这对于在规则集中跨多个相关规则应用一致的操作符更改非常有用。


**示例:**


```apache
# 更新所有 SQL 注入规则以使用新模式
SecRuleUpdateOperatorByTag "attack-sqli" "@rx (?i)(union|select|insert|update|delete)"

# 更新所有 XSS 检测规则
SecRuleUpdateOperatorByTag "attack-xss" "@rx <script[^>]*>"
```
