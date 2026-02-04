+++
title = "t:removeWhitespace"
weight = 19
+++

**描述:** 移除空白字符

**语法:** `t:removeWhitespace`

**示例:**

```apache
SecRule ARGS "@rx test" "id:1001,t:removeWhitespace"
```


**参数类型:** `string`


成功匹配模式后，最多将复制 100 个捕获项，每个捕获项的名称由 0 至 99 的数字组成。变量 TX.0 始终包含正则表达式匹配到的完整区域。其余所有变量均包含捕获值，其顺序与正则表达式中捕获括号的出现顺序一致。
