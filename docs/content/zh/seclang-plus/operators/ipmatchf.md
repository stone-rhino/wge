+++
title = "@ipMatchF"
weight = 19
+++

**描述:** ipMatchFromFile的简称


**语法:** `@ipMatchF file_path`

**输入数据类型:** `string`

**是否实现:** 否

此指令WGE可以正常解析，但是暂未实现该功能。

**示例:**
```apache
# 从文件加载 IP 黑名单
SecRule REMOTE_ADDR "@ipMatchF /etc/wge/ip-blacklist.txt" \
    "id:1107,phase:1,deny,msg:'IP 在黑名单中'"

# 从文件加载可信代理 IP 列表
SecRule REMOTE_ADDR "@ipMatchF /etc/wge/trusted-proxies.txt" \
    "id:1108,phase:1,pass,nolog,setvar:tx.is_trusted_proxy=1"
```