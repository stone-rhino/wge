+++
title = "SecDataDir"
weight = 15
+++

**描述:** 配置持久化数据存储目录。


**语法:** `SecDataDir PATH`


**区分大小写:** 是



此目录用于存储持久化数据，如 IP 信誉记录、会话数据、用户跟踪数据等。WAF 使用该目录保存需要跨请求持久化的集合数据（通过 initcol 动作初始化）。该目录需对运行 WAF 进程的用户具有可写权限。


**示例:**


```
SecDataDir /var/lib/wge/
```
