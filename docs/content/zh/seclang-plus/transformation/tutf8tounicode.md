+++
title = "t:utf8ToUnicode"
weight = 32
+++

**描述:** UTF-8 转 Unicode

**语法:** `t:utf8ToUnicode`

将所有 UTF-8 字符序列转换为 Unicode 格式（'%uHHHH'）。这有助于输入规范化，特别是对于非英语语言，可以减少误报和漏报。

**示例:**

```apache
SecRule ARGS "@rx test" "id:1001,t:utf8ToUnicode"
```

**参数类型:** `string`
