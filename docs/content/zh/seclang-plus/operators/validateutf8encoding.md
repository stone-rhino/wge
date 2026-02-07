+++
title = "@validateUtf8Encoding"
weight = 23
+++

**描述:** 验证输入数据是否为 UTF-8 编码格式

**语法:** `@validateUtf8Encoding parameter`

**输入数据类型:** `string`

**是否实现:** 否

检测输入数据中是否包含非 UTF-8 编码格式的内容，如果存在则匹配成功。

此指令WGE可以正常解析，但是暂未实现该功能。

**示例:**
```apache
SecRule ARGS "@validateUtf8Encoding parameter" "id:1001,deny,msg:'Test'"
```
