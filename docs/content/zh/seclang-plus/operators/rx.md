+++
title = "@rx"
weight = 1
+++

**描述:** 使用 Perl 兼容正则表达式 (PCRE) 对变量值进行匹配


**语法:** `"@rx pattern"`


@rx 是 SecLang 中最强大和最常用的操作符。它使用 PCRE 语法，支持所有标准正则表达式特性。如果未指定操作符，@rx 将作为默认操作符使用。


**示例:**


```apache
# 检测 SQL 注入关键字
SecRule ARGS "@rx (?i:select|union|insert|update|delete|drop)" \
    "id:1001,phase:2,deny,msg:'SQL Injection detected'"

# 检测 XSS 攻击模式
SecRule ARGS "@rx ]*>.*?" \
    "id:1002,phase:2,deny,msg:'XSS Attack detected'"
```


**参数类型:** `string (正则表达式)`
