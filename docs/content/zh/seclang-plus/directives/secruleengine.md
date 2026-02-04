+++
title = "SecRuleEngine"
weight = 1
+++

**描述:** 配置规则引擎的运行模式。


**语法:** `SecRuleEngine On|Off|DetectionOnly`


**默认值:** Off


**版本:** 1.0



可选值:


- **On:** 启用规则引擎,执行所有规则并应用破坏性动作
- **Off:** 禁用规则引擎,不执行任何规则
- **DetectionOnly:** 仅检测模式,执行规则但不应用破坏性动作

**区分大小写:** 是



**示例:**


```apache
SecRuleEngine On
```
