+++
title = "deny"
weight = 10
+++

**描述:** 拒绝请求


**语法:** `deny`


deny 是最常用的破坏性动作，用于立即终止当前请求的处理并返回错误响应。当规则匹配时，WGE 将停止处理后续规则，并向客户端返回指定的 HTTP 状态码（默认 403 Forbidden）。


deny 动作通常与 status 动作配合使用，以指定自定义的响应状态码。在异常评分模式中，deny 通常只用于最终的评分阈值检查规则。


**示例:**


```apache
# 基本拒绝 - 返回默认 403 状态码
SecRule ARGS "@detectSQLi" "id:1,phase:2,deny,msg:'SQL 注入检测'"

# 指定自定义状态码
SecRule REQUEST_URI "@contains ../../../" \
    "id:2,phase:1,deny,status:400,msg:'路径遍历攻击'"

# 异常评分模式中的阈值检查
SecRule TX:anomaly_score "@ge 5" \
    "id:3,phase:2,deny,status:403,msg:'异常评分超标: %{TX.anomaly_score}'"

# 返回 503 表示服务暂时不可用
SecRule IP:request_count "@gt 1000" \
    "id:4,phase:1,deny,status:503,msg:'请求频率限制'"
```


**参数类型:** `无`


**区分大小写:** 是
