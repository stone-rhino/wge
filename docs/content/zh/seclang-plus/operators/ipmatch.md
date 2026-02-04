+++
title = "@ipMatch"
weight = 18
+++

**描述:** IP 地址/网段匹配


**语法:** `"@ipMatch ip1,ip2,cidr1,cidr2..."`


@ipMatch 检查变量值是否匹配指定的 IP 地址或 CIDR 网段。支持 IPv4 和 IPv6 地址。多个地址/网段用逗号分隔。常用于实现 IP 白名单、黑名单、地域限制等访问控制场景。匹配效率高于使用 @rx 进行 IP 匹配。


**示例:**


```apache
# IP 白名单 - 允许特定 IP 跳过检测
SecRule REMOTE_ADDR "@ipMatch 192.168.1.0/24,10.0.0.0/8" \
    "id:1103,phase:1,pass,nolog,ctl:ruleEngine=Off"

# IP 黑名单 - 阻止已知恶意 IP
SecRule REMOTE_ADDR "@ipMatch 1.2.3.4,5.6.7.8" \
    "id:1104,phase:1,deny,msg:'已列入黑名单的 IP 地址'"

# 限制管理后台访问来源
SecRule REQUEST_URI "@beginsWith /admin" \
    "id:1105,phase:1,chain"
    SecRule REMOTE_ADDR "!@ipMatch 192.168.1.0/24" \
        "deny,msg:'禁止从外部网络访问管理后台'"

# 检测内网 IP（可能的 SSRF 攻击）
SecRule ARGS "@ipMatch 127.0.0.0/8,10.0.0.0/8,172.16.0.0/12,192.168.0.0/16" \
    "id:1106,phase:2,deny,msg:'检测到内网 IP 访问，疑似 SSRF'"
```


**参数类型:** `string`


**区分大小写:** 是
