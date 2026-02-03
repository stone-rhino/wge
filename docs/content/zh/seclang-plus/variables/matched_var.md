+++
title = "MATCHED_VAR"
weight = 87
+++

**描述:** 最后匹配的变量值


**语法:** `MATCHED_VAR`


MATCHED_VAR 包含规则中最后一个匹配成功的变量的值。当规则检查多个变量（如 ARGS）并找到匹配时，此变量保存触发匹配的具体值。常用于日志记录和调试，了解具体什么内容触发了规则。


**示例:**


```apache
# 在日志中记录匹配的内容
SecRule ARGS "@detectSQLi" \
    "id:1063,phase:2,deny,msg:'SQL 注入: %{MATCHED_VAR}'"
```
