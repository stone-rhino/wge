+++
title = "t:removeCommentsChar"
weight = 22
+++

**描述:** 移除注释字符

**语法:** `t:removeCommentsChar`

**示例:**

```apache
SecRule ARGS "@rx test" "id:1001,t:removeCommentsChar"
```


**参数类型:** `string`


移除常见注释字符（/*, */, --, #）。
