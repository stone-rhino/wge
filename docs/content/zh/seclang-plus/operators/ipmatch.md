+++
title = "@ipMatch"
weight = 18
+++

**描述:** IP 地址/网段匹配

**语法:** `@ipMatch ip|CIDR`

**输入数据类型:** `string`

@ipMatch 检查变量值是否匹配指定的 IP 地址或 CIDR 网段。支持 IPv4 和 IPv6 地址。常用于实现 IP 白名单、黑名单、地域限制等访问控制场景。

**示例:**

```apache
# IP 黑名单 - 阻止已知恶意 IP
SecRule REMOTE_ADDR "@ipMatch  2001:db8:85a3:8d3:1319:8a2e:370:7348" \
    "id:1104,phase:1,deny,msg:'已列入黑名单的 IP 地址'"

SecRule REMOTE_ADDR "@ipMatch 10.10.50.0/24" "id:162"
```