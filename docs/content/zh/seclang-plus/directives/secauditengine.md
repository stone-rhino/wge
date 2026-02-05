+++
title = "SecAuditEngine"
weight = 50
+++

**描述:** 配置审计日志引擎。


**语法:** `SecAuditEngine On|Off|RelevantOnly`


**默认值:** Off

**可选值:**


- **On:** 对所有事务启用审计日志
- **Off:** 禁用审计日志
- **RelevantOnly:** 仅记录触发警告/错误或匹配 SecAuditLogRelevantStatus 配置的状态码的事务

**区分大小写:** 是

**是否实现:** 否


**示例:**


```apache
SecAuditEngine RelevantOnly
```
