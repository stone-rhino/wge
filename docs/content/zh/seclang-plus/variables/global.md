+++
title = "GLOBAL"
weight = 82
+++

**描述:** 全局变量集合


**语法:** `GLOBAL:key`


GLOBAL 集合用于存储跨事务的全局持久化数据。与 TX 不同，GLOBAL 变量在多个请求之间保持，可用于实现全局计数器、全局状态标志等。需要通过 initcol 动作初始化后才能使用。


**示例:**


```apache
# 初始化全局集合
SecAction "id:1055,phase:1,pass,nolog,initcol:GLOBAL=global"

# 全局请求计数
SecRule REQUEST_URI "@rx ." \
    "id:1056,phase:1,pass,nolog,setvar:GLOBAL.request_count=+1"
```
