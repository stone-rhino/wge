+++
title = "SecAuditLogRelevantStatus"
weight = 57
+++

**描述:** 配置哪些 HTTP 响应状态码被视为审计日志的相关状态码。


**语法:** `SecAuditLogRelevantStatus REGEX`


**区分大小写:** 是

**是否实现:** 否


此指令定义用于匹配 HTTP 响应状态码的正则表达式模式。当 SecAuditEngine 设置为 RelevantOnly 时，只有匹配响应状态码的事务才会被记录。


**示例:**


```apache
SecAuditLogRelevantStatus "^(?:5|4(?!04))"
```

此示例记录所有 5xx 错误和除 404 外的 4xx 错误。
