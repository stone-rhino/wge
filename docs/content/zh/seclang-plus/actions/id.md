+++
title = "id"
weight = 1
+++

**描述:** 规则唯一标识符 (必需)

**语法:** `id: [']? INT [']?`

**区分大小写:** 是

对于一个SecRule规则，其必须存在一个 id 动作，且这个必须在最上层父规则的动作中，用于作为其唯一标识符，id 必须是正整数。

**示例:**

```apache
SecRule ARGS "@rx test" "id:1001,deny"
```