+++
title = "skip"
weight = 42
+++

**描述:** 跳过指定数量的规则

**语法:** `skip:INT`

**区分大小写:** 是

skip 动作在当前规则匹配时跳过配置中接下来的多条规则。这提供了一种简单的方式来有条件地绕过规则，而无需使用链或标记。

**注意：**
- skip 仅在同一阶段内有效，若当前阶段剩余未执行规则小于 skip 的数量时仅会跳过当前阶段剩余所有规则
- skip 对于父子规则当作一条而不是分别计数
- 对于复杂的条件逻辑，建议使用 `skipAfter` 配合 `SecMarker`

**示例:**

```apache
# 如果请求来自可信 IP，跳过接下来的 2 条规则
SecRule REMOTE_ADDR "@ipMatch 10.0.0.0/8" \
    "id:100,phase:1,pass,nolog,skip:2"
SecRule ARGS "@detectSQLi" "id:101,phase:1,deny,status:403,msg:'SQL 注入'"
SecRule ARGS "@detectXSS" "id:102,phase:1,deny,status:403,msg:'XSS 攻击'"

# 对静态资源跳过认证检查
SecRule REQUEST_URI "\.(css|js|png|jpg)$" \
    "id:200,phase:1,pass,nolog,skip:1"
SecRule REQUEST_HEADERS:Authorization "!@rx ^Bearer " \
    "id:201,phase:1,deny,status:401,msg:'缺少认证令牌'"
```