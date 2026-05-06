+++
title = "SecAuditLogDirMode"
weight = 53
+++

**描述:** 配置并发审计日志目录的目录权限模式。


**语法:** `SecAuditLogDirMode OCTAL`


**默认值:** 0600

**区分大小写:** 是

**是否实现:** 否


此指令设置使用并发审计日志时创建目录的 Unix 权限模式。值必须以八进制格式指定。


**示例:**


```apache
SecAuditLogDirMode 0750
```
