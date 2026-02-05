+++
title = "SecAuditLogFileMode"
weight = 55
+++

**描述:** 配置审计日志文件的文件权限模式。


**语法:** `SecAuditLogFileMode OCTAL`


**默认值:** 0600

**区分大小写:** 是

**是否实现:** 否


此指令设置使用并发审计日志时创建的审计日志文件的 Unix 权限模式。值必须以八进制格式指定。


**示例:**


```apache
SecAuditLogFileMode 0640
```
