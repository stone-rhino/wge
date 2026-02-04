+++
title = "t:utf8ToUnicode"
weight = 32
+++

**描述:** UTF-8 转 Unicode

**语法:** `t:utf8ToUnicode`

**示例:**

```apache
SecRule ARGS "@rx test" "id:1001,t:utf8ToUnicode"
```


**参数类型:** `string`


将所有 UTF-8 字符序列转换为 Unicode（采用 '%uHHHH' 格式）。此功能可帮助输入规范化，特别适用于非英语语言，最大限度减少误报和漏报。
