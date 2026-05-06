+++
title = "SecAuditLog2"
weight = 52
+++

**描述:** 配置辅助审计日志文件的路径。


**语法:** `SecAuditLog2 /path/to/audit2.log`


**区分大小写:** 是

**是否实现:** 否


此指令定义辅助审计日志文件的路径，通常用于并发日志记录或作为备份目标。


**示例:**


```apache
SecAuditLog2 /var/log/modsec_audit2.log
```
