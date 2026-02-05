+++
title = "skip"
weight = 42
+++

**描述:** 跳过指定数量的规则

**语法:** `skip:N`

skip 动作在当前规则匹配时跳过配置中接下来的 N 条规则。这提供了一种简单的方式来有条件地绕过规则，而无需使用链或标记。

重要说明：
- N 必须是正整数
- skip 仅在同一阶段内有效
- skip 计数规则，而非链（每条链接的规则计为一条规则）
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

**参数类型:** `integer`

**区分大小写:** 是
