+++
title = "@fuzzyHash"
weight = 34
+++

**描述:** 模糊哈希匹配


**语法:** `@fuzzyHash file_path num`

**输入数据类型:** `string`

**是否实现:** 否

使用模糊哈希，比较输入数据和指定文件中的哈希值进行比较，如果大于等于指定的阈值（num），那么就算匹配成功。

此指令WGE可以正常解析，但是暂未实现该功能。

**示例:**
```apache
SecRule ARGS "@fuzzyHash parameter" "id:1001,deny,msg:'Test'"
```