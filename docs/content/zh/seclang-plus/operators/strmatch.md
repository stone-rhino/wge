+++
title = "@strmatch"
weight = 8
+++

**描述:** 单个模式的字符串匹配，使用Boyer-Moore-Horspool算法。

**语法:** `@strmatch string`

**输入数据类型:** `string`

**是否实现:** 否

此指令WGE可以正常解析，但是暂未实现该功能

**示例:**
```apache
SecRule ARGS "@strmatch admin" "id:1001,deny,msg:'Test'"
```