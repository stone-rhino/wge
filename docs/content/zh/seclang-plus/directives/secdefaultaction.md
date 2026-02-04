+++
title = "SecDefaultAction"
weight = 39
+++

**描述:** 定义特定阶段的默认动作列表。


**语法:** `SecDefaultAction "ACTIONS"`


**默认值:** phase:2,log,auditlog,pass


**版本:** 1.0



**区分大小写:** 是



在同一配置上下文中，所有位于先前 SecDefaultAction 指令之后的规则将继承其设置，除非规则自身指定了更具体的动作。每个 SecDefaultAction 指令必须指定破坏性动作和处理阶段，且不能包含元数据动作。需注意 SecDefaultAction 不会跨配置上下文继承。


**示例:**


```
SecDefaultAction "phase:1,log,auditlog,pass"
SecDefaultAction "phase:2,log,auditlog,deny,status:403"
```
