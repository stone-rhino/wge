+++
title = "MATCHED_VARS_NAMES"
weight = 90
+++

**描述:** 所有匹配的变量名


**语法:** `MATCHED_VARS_NAMES`


MATCHED_VARS_NAMES 是一个集合变量，包含当前规则所有匹配成功的变量名称。与 MATCHED_VAR_NAME 不同，当规则匹配多个变量时，此变量包含所有匹配变量的名称。


**示例:**


```apache
# 记录所有匹配的变量名
SecRule ARGS|REQUEST_HEADERS "@detectSQLi" \
    "id:1066,phase:2,deny,msg:'SQL 注入在: %{MATCHED_VARS_NAMES}'"
```
