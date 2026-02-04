+++
title = "SecRequestBodyLimitAction"
weight = 5
+++

**描述:** 配置当请求体超过 SecRequestBodyLimit 限制时的处理方式。


**语法:** `SecRequestBodyLimitAction Reject|ProcessPartial`


**默认值:** Reject


**可选值:**


- **Reject:** 拒绝超限请求
- **ProcessPartial:** 仅检查符合限制的部分请求体，放行其余内容

**区分大小写:** 是



当 WAF 处于 DetectionOnly 模式且需完全被动运行时，使用 ProcessPartial 可避免干扰事务。可通过创建规则检查 INBOUND_DATA_ERROR 变量来检测请求体是否超限。


**示例:**


```
SecRequestBodyLimitAction Reject
```
