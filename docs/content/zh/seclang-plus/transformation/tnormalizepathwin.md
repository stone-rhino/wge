+++
title = "t:normalizePathWin"
weight = 27
+++

**描述:** 规范化 Windows 路径

**语法:** `t:normalizePathWin`

**示例:**

```apache
SecRule ARGS "@rx test" "id:1001,t:normalizePathWin"
```


**参数类型:** `string`


功能与 normalizePath 相同，但会先将反斜杠转换为正斜杠。
