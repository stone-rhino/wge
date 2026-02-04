+++
title = "IP"
weight = 83
+++

**描述:** IP 地址集合


**语法:** `IP:key`


IP 集合用于存储与客户端 IP 地址关联的持久化数据。常用于实现 IP 级别的访问频率限制、IP 信誉评分等。需要通过 initcol:IP=%{REMOTE_ADDR} 初始化。IP 集合中的数据在同一客户端 IP 的多次请求间保持。


**示例:**


```apache
# 初始化 IP 集合
SecAction "id:1057,phase:1,pass,nolog,initcol:IP=%{REMOTE_ADDR}"

# IP 级别的请求频率限制
SecRule IP:request_count "@gt 100" \
    "id:1058,phase:1,deny,msg:'IP 请求频率超限'"
SecRule REQUEST_URI "@rx ." \
    "id:1059,phase:1,pass,nolog,setvar:IP.request_count=+1,\
    expirevar:IP.request_count=60"
```
