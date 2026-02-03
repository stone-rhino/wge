+++
title = "REMOTE_ADDR"
weight = 31
+++

**描述:** 客户端 IP 地址


**语法:** `REMOTE_ADDR`


REMOTE_ADDR 包含发起请求的客户端 IP 地址。这是最常用的变量之一，用于实现 IP 黑白名单、访问频率控制等安全策略。常与 @ipMatch 操作符配合使用来匹配 IP 地址或 CIDR 网段。


**示例:**


```apache
# IP 白名单 - 跳过内网 IP 的检查
SecRule REMOTE_ADDR "@ipMatch 10.0.0.0/8,172.16.0.0/12,192.168.0.0/16" \
    "id:1038,phase:1,pass,nolog,allow"

# IP 黑名单 - 阻止特定 IP
SecRule REMOTE_ADDR "@ipMatch 1.2.3.4,5.6.7.8" \
    "id:1039,phase:1,deny,status:403,msg:'IP 已被封禁'"
```
