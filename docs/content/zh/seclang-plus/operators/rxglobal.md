+++
title = "@rxGlobal"
weight = 12
+++

**描述:** 全局正则表达式匹配

**语法:** `@rxGlobal pattern`

**输入数据类型:** `string`

**是否实现:** 否

rxGlobal 操作符与 rx 不同的是 rx 匹配成功一次就结束，而 rxGlobal 会从上次匹配成功结束的位置继续尝试匹配。

此指令WGE可以正常解析，但是暂未实现该功能。

**示例:**

```apache
SecRule ARGS "@rxGlobal ^admin" "id:1001,deny,msg:'Test'"
```