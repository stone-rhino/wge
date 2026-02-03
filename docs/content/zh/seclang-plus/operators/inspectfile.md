+++
title = "@inspectFile"
weight = 33
+++

**描述:** 调用外部脚本检查文件


**语法:** `"@inspectFile parameter"`


**示例:**


```apache
SecRule ARGS "@inspectFile parameter" "id:1001,deny,msg:'Test'"
```


**参数类型:** `string (文件路径)`


**是否实现:** 暂无
