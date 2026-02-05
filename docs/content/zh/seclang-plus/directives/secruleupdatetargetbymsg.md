+++
title = "SecRuleUpdateTargetByMsg"
weight = 45
+++

**描述:** 根据消息内容更新规则的参数列表。


**语法:** `SecRuleUpdateTargetByMsg "MESSAGE" VARIABLES`


**区分大小写:** 是

与 SecRuleUpdateTargetById 类似，SecRuleUpdateTargetByMsg 是通过msg修改指定规则的变量，更多注意事项可见 SecRuleUpdateTargetById。

**示例:**


```apache
SecRuleUpdateTargetByMsg "SQL Injection" !ARGS:id
```
