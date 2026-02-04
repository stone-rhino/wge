+++
title = "t:normalisePathWin"
weight = 28
+++

**描述:** 规范化 Windows 路径 (英式拼写)

**语法:** `t:normalisePathWin`

**示例:**

```apache
SecRule ARGS "@rx test" "id:1001,t:normalisePathWin"
```


**参数类型:** `string`


参见 normalizePathWin。
