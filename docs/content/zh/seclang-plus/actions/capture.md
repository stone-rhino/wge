+++
title = "capture"
weight = 23
+++

**描述:** 设置捕获标识。

**语法:** `capture`

**区分大小写:** 是

设置当前规则（不包括父子规则）是否捕获，当规则存在 capture 时匹配完成后会将捕获的内容写入 TX 变量中，常见支持捕获的运算符有rx、within等，detectSQLi 和 detectXSS 也支持，但是其存储的是生成的指纹在`TX:0`中。

正则匹配会根据其捕获组的数量依次存储在`TX:0`、`TX:1`……中（最多100个）。

**示例:**

```apache
SecRule ARGS "@rx test" "id:1001,capture,status:403"
```