+++
title = "SecResponseBodyLimitAction"
weight = 24
+++

**描述:** 配置当响应体超过 SecResponseBodyLimit 限制时的处理方式。


**语法:** `SecResponseBodyLimitAction Reject|ProcessPartial`


**默认值:** ProcessPartial


**可选值:**


- **Reject:** 拒绝超限响应
- **ProcessPartial:** 仅检查符合限制的部分响应体，放行其余内容

**区分大小写:** 是

**是否实现:** 否

此指令WGE可以正常解析，但是暂未实现该功能。


**示例:**


```apache
SecResponseBodyLimitAction ProcessPartial
```
