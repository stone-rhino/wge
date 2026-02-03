+++
title = "@noMatch"
weight = 32
+++

**描述:** 不匹配 (总是返回 false)


**语法:** `"@noMatch"`


@noMatch 无条件返回匹配失败（false）。规则永远不会触发其动作。此操作符主要用于调试目的，可以临时禁用规则而无需注释或删除规则代码。也可用于创建占位规则或在测试环境中暂时关闭特定检测。


**示例:**


```apache
# 临时禁用规则（用于调试）
SecRule ARGS "@noMatch" \
    "id:1119,phase:2,deny,msg:'此规则已禁用'"

# 创建占位规则，保留规则 ID
SecRule REQUEST_URI "@noMatch" \
    "id:1120,phase:1,deny,msg:'保留规则 ID - 待实现'"

# 在测试环境中禁用某些检测
SecRule TX:testing_mode "@eq 1" \
    "id:1121,phase:1,chain,pass"
    SecRule ARGS "@noMatch" \
        "deny,msg:'测试模式下此检测已禁用'"
```


**参数类型:** `无`


**区分大小写:** 是
