+++
title = "@rbl"
weight = 36
+++

**描述:** 在作为参数给出的RBL（real-time block list）中查找输入值。该参数可以是IPv4地址或主机名。

**语法:** `@rbl string`

**输入数据类型:** `string`

**是否实现:** 否

此指令WGE可以正常解析，但是暂未实现该功能。

**示例:**

```apache
SecRule ARGS "@rbl sbl-xbl.spamhaus.org" "id:1001,deny,msg:'Test'"
```
