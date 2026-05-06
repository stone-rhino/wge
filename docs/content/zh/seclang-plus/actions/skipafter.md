+++
title = "skipAfter"
weight = 43
+++

**描述:** 跳转到指定标记

**语法:** `skipAfter:MARKER_NAME`

**区分大小写:** 是

skipAfter 动作在当前规则匹配时跳转到指定的命名标记（使用 `SecMarker` 创建）。这为条件性规则执行提供了灵活的流程控制，特别适用于复杂的规则集。

相比 `skip` 的优势：
- 更易读和维护
- 与规则数量无关（可以添加/删除规则而不破坏跳转逻辑）
- 更适合条件性绕过规则组
- 配置中意图更清晰

**示例:**

```apache
# 对可信路径跳过所有 SQL 注入检查
SecRule REQUEST_URI "^/api/internal/" \
    "id:100,phase:2,pass,nolog,skipAfter:END_SQLI_CHECKS"
SecRule ARGS "@detectSQLi" "id:101,phase:2,deny,status:403,msg:'SQL 注入'"
SecRule ARGS "@rx union.*select" "id:102,phase:2,deny,status:403,msg:'SQL Union'"
SecRule ARGS "@rx (\%27)|(\')" "id:103,phase:2,deny,status:403,msg:'SQL 引号'"
SecMarker END_SQLI_CHECKS

# 条件性认证绕过
SecRule REQUEST_URI "^/public/" \
    "id:200,phase:1,pass,nolog,skipAfter:AFTER_AUTH"
SecRule REQUEST_HEADERS:X-API-Key "!@streq secret123" \
    "id:201,phase:1,deny,status:403,msg:'无效的 API 密钥'"
SecMarker AFTER_AUTH
```