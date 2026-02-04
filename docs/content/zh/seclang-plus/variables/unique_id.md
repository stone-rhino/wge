+++
title = "UNIQUE_ID"
weight = 91
+++

**描述:** 唯一请求标识符


**语法:** `UNIQUE_ID`


UNIQUE_ID 包含 WGE 为每个请求生成的唯一标识符。此 ID 在日志中用于关联同一请求的多条记录，便于事后分析和调试。可在 msg 动作中引用以便追踪特定请求。


**示例:**


```apache
# 在日志中记录请求 ID
SecRule ARGS "@detectSQLi" \
    "id:1067,phase:2,deny,msg:'[%{UNIQUE_ID}] SQL 注入攻击'"
```
