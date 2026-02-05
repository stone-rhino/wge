+++
title = "SecStatusEngine"
weight = 19
+++

**描述:** 配置状态引擎，用于收集和报告运行时统计信息。


**语法:** `SecStatusEngine On|Off`


**默认值:** Off


**区分大小写:** 是

**是否实现:** 否

启用后，状态引擎会收集 WAF 运行时的统计数据，包括处理的请求数、规则匹配次数、阻断次数等。这些信息有助于监控 WAF 性能和安全态势。

此指令WGE可以正常解析，但是暂未实现该功能。

**示例:**


```apache
SecStatusEngine On
```
