+++
title = "t:trim"
weight = 36
+++

**描述:** 去除首尾空白

**语法:** `t:trim`

**示例:**

```apache
SecRule ARGS "@rx test" "id:1001,t:trim"
```


**参数类型:** `string`


同时清除输入字符串左右两侧的空格。
