+++
title = "SESSION"
weight = 84
+++

**描述:** 会话变量集合


**语法:** `SESSION:key`


SESSION 集合用于存储与用户会话关联的持久化数据。需要先通过 setsid 动作设置会话标识符，然后通过 initcol:SESSION=%{SESSIONID} 初始化。可用于跟踪会话级别的行为、实现会话级访问控制等。


**示例:**


```apache
# 设置会话 ID 并初始化
SecRule REQUEST_COOKIES:PHPSESSID "@rx (.+)" \
    "id:1060,phase:1,pass,nolog,setsid:%{TX.1},initcol:SESSION=%{SESSIONID}"
```
