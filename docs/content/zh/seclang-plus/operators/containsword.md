+++
title = "@containsWord"
weight = 6
+++

**描述:** 判断输入是否包含指定字符串（同时判断单词边界）。


**语法:** `@containsWord string`

**是否实现:** 否

**输入数据类型:** `string`

此指令WGE可以正常解析，但是暂未实现该功能。

**示例:**

```apache
SecRule ARGS "@containsWord admin" "id:1001,deny,msg:'Test'"
```
