+++
title = "t:normalizePath"
weight = 25
+++

**描述:** 规范化路径

**语法:** `t:normalizePath`

**示例:**

```apache
SecRule ARGS "@rx test" "id:1001,t:normalizePath"
```


**参数类型:** `string`


移除输入字符串中的多重斜杠、目录自引用及目录后向引用（位于输入开头的情况除外）。
