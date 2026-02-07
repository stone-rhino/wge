+++
title = "@rx"
weight = 1
+++

**描述:** 使用 re2 库进行正则匹配。

**语法:** `@rx pattern`

**输入数据类型:** `string`

@rx 是 SecLang 中最强大和最常用的操作符。在默认情况其使用 re2 进行正则匹配，但是当 re2 编译失败的情况下其会尝试使用 pcre2。

当不存在operator关键字的时候会默认使用正则操作符。


**示例:**
```apache
# 检测 SQL 注入关键字
SecRule ARGS "@rx (?i:select|union|insert|update|delete|drop)" \
    "id:1001,phase:2,deny,msg:'SQL Injection detected'"

# 检测 XSS 攻击模式
SecRule ARGS "@rx ]*>.*?" \
    "id:1002,phase:2,deny,msg:'XSS Attack detected'"

# 没有 operator 关键字的情况下使用正则
SecRule ARGS "admin" "id:1003,phase:2,deny,msg:'test'"
```