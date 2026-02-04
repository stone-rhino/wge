+++
title = "RULE"
weight = 69
+++

**描述:** 当前规则信息


**语法:** `RULE`


RULE 变量提供对当前执行规则元数据的访问。可以通过子键访问具体属性，如 RULE.id（规则ID）、RULE.msg（规则消息）、RULE.severity（严重级别）等。主要用于在日志记录和动态消息中引用规则自身的信息。


**示例:**


```apache
# 在日志中引用规则信息
SecRule ARGS "@detectSQLi" \
    "id:1047,phase:2,deny,msg:'规则 %{RULE.id} 触发: SQL 注入检测',\
    severity:CRITICAL,tag:'attack-sqli'"
```
