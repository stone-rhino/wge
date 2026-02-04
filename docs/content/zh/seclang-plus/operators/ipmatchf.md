+++
title = "@ipMatchF"
weight = 19
+++

**描述:** 从文件加载 IP 地址列表进行匹配 (@ipMatchFromFile 的别名)


**语法:** `"@ipMatchF /path/to/ip-list.txt"`


@ipMatchF 是 @ipMatchFromFile 的简写形式，从外部文件加载 IP 地址/网段列表进行匹配。文件中每行一个 IP 地址或 CIDR 网段，以 # 开头的行作为注释忽略。适用于管理大量 IP 地址的场景，如动态更新的黑名单、CDN 节点列表等。


**示例:**


```apache
# 从文件加载 IP 黑名单
SecRule REMOTE_ADDR "@ipMatchF /etc/wge/ip-blacklist.txt" \
    "id:1107,phase:1,deny,msg:'IP 在黑名单中'"

# 从文件加载可信代理 IP 列表
SecRule REMOTE_ADDR "@ipMatchF /etc/wge/trusted-proxies.txt" \
    "id:1108,phase:1,pass,nolog,setvar:tx.is_trusted_proxy=1"
```


**文件格式示例 (/etc/wge/ip-blacklist.txt):**


```
# 恶意 IP 黑名单
# 更新日期: 2024-01-01
1.2.3.4
5.6.7.8
10.0.0.0/8
192.168.100.0/24
```


**参数类型:** `string (文件路径)`


**区分大小写:** 是
