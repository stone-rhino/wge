+++
title = "SecRuleUpdateActionById"
weight = 43
+++

**描述:** 根据 ID 更新规则的动作。


**语法:** `SecRuleUpdateActionById INT| (INT ':' '-'? INT) "ACTIONS"`


**区分大小写:** 是



该指令将用第二个参数提供的动作覆盖指定规则的动作列表。存在两项限制：无法用于修改规则 ID 或阶段；仅覆盖只能出现一次的动作。允许在列表中多次出现的动作将追加至列表末尾。
前例中最终生效的规则如下：
添加 t:none 将覆盖先前指定的任何转换函数（如示例中的 t:lowercase）。

对于存在子规则的，可以使用SecRuleUpdateActionById id:0来更新子规则的action，其中第二个INT的值为-1代表最上层父规则，0为深度为1的子规则……

**示例:**


```apache
SecRuleUpdateActionById 1001 "pass,log"
```
