+++
title = "SecDefaultAction"
weight = 39
+++

**描述:** 定义特定阶段的默认动作列表。


**语法:** `SecDefaultAction "ACTIONS"`


**区分大小写:** 是


SecDefaultAction中设置了一系列动作，这个动作会被同阶段的所有规则继承，由此SecDefaultAction中必须包含phase，若同一阶段存在多个SecDefaultAction，那么后面的会取代前面的。SecDefaultAction主要影响三方面：

1. **转化函数**：SecDefaultAction中的转换函数会被所有同阶段的所有规则执行，且在其自己的转换函数之前执行，除非其存在t:none；
2. **阻断性动作**：使用**block**的规则会根据同阶段的DefaultAction是否存在阻断性动作（如allow、deny）来决定自己是否阻断；
3. **匹配后动作**：如setvar、ctl等动作，所有同阶段的规则匹配成功会首先执行DefaultAction再执行自己的动作。


**示例:**


```apache
SecDefaultAction "phase:1,log,auditlog,pass"
SecDefaultAction "phase:2,log,auditlog,deny,status:403"
```
