+++
title = "@ipMatchFromFile"
weight = 20
+++

**描述:** 从文件加载 IP 地址列表匹配

**语法:** `@ipMatchFromFile file_path`

**输入数据类型:** `string`

**是否实现:** 否

此指令WGE可以正常解析，但是暂未实现该功能。

**示例:**

```apache
SecRule ARGS "@ipMatchFromFile 192.168.1.1" "id:1001,deny,msg:'Test'"
```