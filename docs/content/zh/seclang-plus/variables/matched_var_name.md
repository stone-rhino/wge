+++
title = "MATCHED_VAR_NAME"
weight = 88
+++

**描述:** 最后匹配的变量名


**语法:** `MATCHED_VAR_NAME`


MATCHED_VAR_NAME 包含最后一个匹配成功的变量的完整名称。例如，如果 ARGS:username 触发了规则，MATCHED_VAR_NAME 的值为 "ARGS:username"。与 MATCHED_VAR 配合使用可以完整记录触发规则的位置和内容。


**示例:**


```apache
# 记录匹配的变量名和值
SecRule ARGS "@detectXSS" \
    "id:1064,phase:2,deny,msg:'XSS 在 %{MATCHED_VAR_NAME}: %{MATCHED_VAR}'"
```
