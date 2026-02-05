+++
title = "SecCollectionTimeout"
weight = 21
+++

**描述:** 配置持久化集合的过期时间 (秒)。


**语法:** `SecCollectionTimeout SECONDS`


**默认值:** 3600


**区分大小写:** 是


**是否实现:** 否


持久化集合（如 IP、SESSION、USER 等）用于跨请求跟踪数据。此指令定义集合记录的存活时间。超过此时间后，集合记录将被自动清理。合理设置此值可平衡安全跟踪需求和存储空间使用。


**示例:**


```apache
SecCollectionTimeout 3600
```
