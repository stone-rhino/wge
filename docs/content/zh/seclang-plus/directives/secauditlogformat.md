+++
title = "SecAuditLogFormat"
weight = 54
+++

**描述:** 配置审计日志的输出格式。


**语法:** `SecAuditLogFormat Native|JSON`


**默认值:** Native

**可选值:**


- **Native:** 使用传统的 ModSecurity 审计日志格式
- **JSON:** 以 JSON 格式输出审计日志条目，便于解析

**区分大小写:** 是

**是否实现:** 否


**示例:**


```apache
SecAuditLogFormat JSON
```
