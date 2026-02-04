+++
title = "t:lowercase"
weight = 2
+++

**描述:** 转换为小写

**语法:** `t:lowercase`

**示例:**

```apache
SecRule ARGS "@rx test" "id:1001,t:lowercase"
```


**参数类型:** `string`


使用当前 C 语言环境设置将所有字符转换为小写。
