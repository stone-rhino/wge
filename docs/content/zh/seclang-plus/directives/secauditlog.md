+++
title = "SecAuditLog"
weight = 51
+++

**描述:** 配置主审计日志文件的路径。


**语法:** `SecAuditLog /path/to/audit.log`


**区分大小写:** 是

**是否实现:** 否


此指令定义引擎写入事务详细信息的主审计日志文件路径。


**示例:**


```apache
SecAuditLog /var/log/modsec_audit.log
```
