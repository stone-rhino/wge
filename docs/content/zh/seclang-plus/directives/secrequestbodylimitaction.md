+++
title = "SecRequestBodyLimitAction"
weight = 5
+++

**描述:** 配置当请求体超过 SecRequestBodyLimit 限制时的处理方式。


**语法:** `SecRequestBodyLimitAction Reject|ProcessPartial`


**默认值:** ProcessPartial


**可选值:**


- **Reject:** 拒绝超限请求
- **ProcessPartial:** 仅检查符合限制的部分请求体，放行其余内容

**区分大小写:** 是

**是否实现:** 否

此指令WGE可以正常解析，但是暂未实现该功能。

也即是说即使请求体的大小超过SecRequestBodyLimit的设置大小也不会对规则执行和WGE内部状态有任何影响。

**示例:**


```apache
SecRequestBodyLimitAction Reject
```
