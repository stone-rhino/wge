+++
title = "@inspectFile"
weight = 33
+++

**描述:** 调用外部脚本（lua）执行自定义的算法和逻辑。


**语法:** `@inspectFile string`

**是否实现:** 否

此指令WGE可以正常解析，但是暂未实现该功能。

**示例:**
```apache
SecRule ARGS "@inspectFile parameter" "id:1001,deny,msg:'Test'"
```