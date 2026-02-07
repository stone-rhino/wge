+++
title = "@lt"
weight = 15
+++

**描述:** 小于


**语法:** `@lt string`

**输入数据类型:** `int`

@lt 检查输入数据是否小于指定的数值。

**示例:**
```apache
# 判断后续规则是否继续执行
SecRule TX:level "@lt 4" "id:1001,phase:1,pass,nolog,skipAfter:END"
```