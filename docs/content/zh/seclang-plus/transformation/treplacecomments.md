+++
title = "t:replaceComments"
weight = 23
+++

**描述:** 替换注释为空格

**语法:** `t:replaceComments`

**示例:**

```apache
SecRule ARGS "@rx test" "id:1001,t:replaceComments"
```


**参数类型:** `string`


将所有 C 风格注释（/* ... */）替换为单个空格（连续多个空格不会被压缩）。未终止的注释也将替换为空格（ASCII 0x20）。但独立存在的注释终止符（*/）不会被处理。
