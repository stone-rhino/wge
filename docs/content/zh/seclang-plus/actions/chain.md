+++
title = "chain"
weight = 40
+++

**描述:** 链接规则

**语法:** `chain`

chain 动作将规则链接在一起，使它们作为单个复合规则运行。当规则被链接时，后续规则仅在链中所有前置规则都匹配时才会执行。这允许进行复杂的多条件检查而无需重复代码。

关键行为：
- 只有链中的最后一条规则可以包含破坏性动作（deny、block、drop 等）
- 元数据动作（id、msg、tag 等）应放在链的第一条规则上
- 如果链中任何规则未能匹配，整个链停止处理
- Phase 必须在所有链接的规则中保持一致

**示例:**

```apache
# 阻止从非本地 IP 访问 /admin
SecRule REQUEST_URI "^/admin" \
    "id:100,phase:1,chain,deny,status:403,msg:'非本地 IP 访问管理页面'"
    SecRule REMOTE_ADDR "!@ipMatch 127.0.0.1,10.0.0.0/8,192.168.0.0/16" ""

# 多条件检查：POST 到 login 且请求体为空
SecRule REQUEST_METHOD "@streq POST" \
    "id:101,phase:2,chain,deny,status:400,msg:'空 POST 请求到登录页面'"
    SecRule REQUEST_URI "@beginsWith /login" "chain"
        SecRule REQUEST_BODY "@eq ''" ""
```

**参数类型:** 无

**区分大小写:** 是
