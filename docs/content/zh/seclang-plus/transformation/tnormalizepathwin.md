+++
title = "t:normalizePathWin"
weight = 27
+++

**描述:** 对字符串进行 ``Linux`` 路径规范化。

同 `t:normalisePathWin`。

**语法:** `t:normalizePathWin`

**是否实现:** 是

**示例:**

```apache
SecRule ARGS "@rx test" "id:1001,t:normalizePathWin"
