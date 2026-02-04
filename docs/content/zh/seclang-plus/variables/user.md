+++
title = "USER"
weight = 85
+++

**描述:** 用户变量集合


**语法:** `USER:key`


USER 集合用于存储与认证用户关联的持久化数据。需要先通过 setuid 动作设置用户标识符并初始化。可用于跟踪特定用户的行为模式、实现用户级别的访问控制。


**示例:**


```apache
# 设置用户 ID
SecRule ARGS_POST:username "@rx (.+)" \
    "id:1061,phase:2,pass,nolog,setuid:%{TX.1}"
```
