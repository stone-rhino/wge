+++
title = "SecResponseBodyLimitAction"
weight = 24
+++

**描述:** 配置当响应体超过 SecResponseBodyLimit 限制时的处理方式。

            
**语法:** `SecResponseBodyLimitAction Reject|ProcessPartial`

            
**默认值:** Reject

            
**可选值:**

            
- **Reject:** 拒绝超限响应
- **ProcessPartial:** 仅检查符合限制的部分响应体，放行其余内容
**区分大小写:** 是

            
            
某些站点会生成超长响应，难以设定合理限制。通过设置为 ProcessPartial，可在不大幅提高内存限制的情况下处理超长响应，但存在部分内容未经检查的风险。

            
**示例:**

            
```
SecResponseBodyLimitAction ProcessPartial
```
