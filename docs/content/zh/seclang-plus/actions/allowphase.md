+++
title = "allow:phase"
weight = 13
+++

**描述:** 允许请求通过当前阶段

**语法:** `allow:phase`

**区分大小写:** 是

allow:phase 动作允许请求跳过当前处理阶段中的剩余规则，但后续阶段的规则仍会继续执行。相比 allow:request 提供了更细粒度的控制。若要允许响应通过，可在 RESPONSE_HEADERS 阶段添加规则并使用 allow。

**示例:**

```apache
SecRule ARGS "@rx test" "id:1001,allow:phase"
```
