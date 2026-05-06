+++
title = "MATCHED_VARS"
weight = 89
+++

**描述:** 所有匹配的变量值


**语法:** `MATCHED_VARS`


MATCHED_VARS 是一个集合变量，包含当前规则所有匹配成功的变量值。与 MATCHED_VAR 不同，当规则匹配多个值时（如多个参数都包含恶意内容），MATCHED_VARS 包含所有这些值。可用于链式规则中进一步检查所有匹配内容。

**注意：** MATCHED_VAR_NAMES 默认拿取的是父规则的匹配成功的值，当其不存在父规则时，其会拿取当前规则的匹配成功的值。

WGE 的 MATCHED_VARS 与 modsecurity 的有所不一致，WGE 的并不会不断累积，且其仅可能访问父规则或者自己的匹配成功的值，例如下面的规则：
```apache
SecRule ARGS "@rx [0-9]+['\"]\s*(and|or)" "id:1001,phase:2,t:none,deny,chain"
    SecRule ARGS "@rx --" "chain,t:none"
        SecRule MATCHED_VARS "@rx select" "t:none"
```

这条规则在 WGE 中 MATCHED_VARS 仅会拿到父规则的匹配成功的值，但是在 modsecurity 中其同时会拿到父规则和父规则的父规则的匹配成功的值，MATCHED_VARS_NAMES 也类似。

**示例:**

```apache
# 检查所有匹配值的长度
SecRule ARGS "@rx script" \
    "id:1065,phase:2,pass,setvar:tx.script_found=1,chain"
SecRule MATCHED_VARS "@gt 100" "t:length,deny,msg:'检测到长脚本内容'"
```
