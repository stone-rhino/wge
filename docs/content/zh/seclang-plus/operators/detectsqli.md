+++
title = "@detectSQLi"
weight = 29
+++

**描述:** 使用 libinjection 库检测 SQL 注入攻击


**语法:** `"@detectSQLi"`


@detectSQLi 使用 libinjection 的 SQL 注入检测引擎，通过词法分析和指纹匹配来识别 SQL 注入模式。相比基于正则表达式的检测，libinjection 具有更低的误报率和更高的检测准确性。此操作符不需要参数。


**示例:**


```apache
# 检测所有参数中的 SQL 注入
SecRule ARGS "@detectSQLi" \
    "id:1001,phase:2,deny,msg:'SQL Injection Attack Detected'"

# 结合变量计数检测
SecRule ARGS|REQUEST_HEADERS "@detectSQLi" \
    "id:1002,phase:2,deny,severity:CRITICAL,\
    msg:'SQL Injection detected in %{MATCHED_VAR_NAME}'"
```


**参数类型:** `无`
