+++
title = "@detectXSS"
weight = 30
+++

**描述:** 使用 libinjection 库检测跨站脚本 (XSS) 攻击


**语法:** `"@detectXSS"`


@detectXSS 使用 libinjection 的 XSS 检测引擎，通过识别 HTML 和 JavaScript 注入模式来检测 XSS 攻击。它能够检测各种类型的 XSS，包括反射型、存储型和 DOM 型 XSS。此操作符不需要参数。


**示例:**


```apache
# 检测参数中的 XSS 攻击
SecRule ARGS "@detectXSS" \
    "id:1001,phase:2,deny,msg:'XSS Attack Detected'"

# 检测请求体中的 XSS
SecRule REQUEST_BODY "@detectXSS" \
    "id:1002,phase:2,deny,severity:CRITICAL,\
    msg:'XSS Attack in request body'"
```


**参数类型:** `无`
