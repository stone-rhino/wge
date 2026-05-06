+++
title = "t:normalisePathWin"
weight = 28
+++

**描述:** 对字符串进行 ``Windows`` 路径规范化。

此转换函数与 `t:normalisePath` 相似，但会先把所有反斜杠 ``\`` 转换为正斜杠 ``/``。

**语法:** `t:normalisePathWin`

**是否实现:** 是

**示例:**

```apache
SecRule ARGS "@rx test" "id:1001,t:normalisePathWin"
