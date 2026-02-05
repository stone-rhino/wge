+++
title = "SecAuditLogType"
weight = 59
+++

**描述:** 配置审计日志的记录机制类型。


**语法:** `SecAuditLogType Serial|Concurrent`


**默认值:** Serial

**可选值:**


- **Serial:** 将所有审计日志条目写入单个文件（由 SecAuditLog 指定），适用于低流量环境
- **Concurrent:** 为每个事务在 SecAuditLogStorageDir 指定的目录中创建单独的文件，适用于高流量环境

**区分大小写:** 是

**是否实现:** 否


**示例:**


```apache
SecAuditLogType Concurrent
```
