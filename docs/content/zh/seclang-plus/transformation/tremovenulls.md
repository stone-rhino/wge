+++
title = "t:removeNulls"
weight = 18
+++

**描述:** 移除空字符

**语法:** `t:removeNulls`

**示例:**

```apache
SecRule ARGS "@rx test" "id:1001,t:removeNulls"
```


**参数类型:** `string`


移除输入中的所有 NUL 字节。
