+++
title = "MATCHED_VARS"
weight = 89
+++

**描述:** 所有匹配的变量值


**语法:** `MATCHED_VARS`


MATCHED_VARS 是一个集合变量，包含当前规则所有匹配成功的变量值。与 MATCHED_VAR 不同，当规则匹配多个值时（如多个参数都包含恶意内容），MATCHED_VARS 包含所有这些值。可用于链式规则中进一步检查所有匹配内容。


**示例:**


```apache
# 检查所有匹配值的长度
SecRule ARGS "@rx script" \
    "id:1065,phase:2,pass,setvar:tx.script_found=1,chain"
SecRule MATCHED_VARS "@gt 100" "t:length,deny,msg:'检测到长脚本内容'"
```
