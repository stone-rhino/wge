+++
title = "block"
weight = 17
+++

**描述：** 使用默认规则结束时动作

**语法：** `block`

**区分大小写：** 是

执行 SecDefaultAction 设置的默认结束时动作，如当设置默认的动作为 deny 时，block 相当于 deny，若没有 SecDefaultAction，则其值为`pass`。

**示例:**

```apache
SecRule ARGS "@rx test" "id:1001,block,status:403"
```