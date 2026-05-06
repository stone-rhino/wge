+++
title = "SecAuditLogStorageDir"
weight = 58
+++

**描述:** 配置并发模式下存储单独审计日志文件的目录。


**语法:** `SecAuditLogStorageDir /path/to/dir`


**区分大小写:** 是

**是否实现:** 否


此指令指定使用并发审计日志（SecAuditLogType Concurrent）时存储单独审计日志文件的目录。每个事务将在此目录中有自己的文件。


**示例:**


```apache
SecAuditLogStorageDir /var/log/modsec_audit
```
