+++
title = "t:base64Decode"
weight = 7
+++

**描述:** Base64 解码


**语法:** `t:base64Decode`


t:base64Decode 解码 Base64 编码的字符串。攻击者经常使用 Base64 编码来隐藏恶意载荷以绕过安全检测。此转换函数在检测前先解码数据，揭示隐藏的攻击模式。


**示例:**


```apache
# 检测 Base64 编码的攻击载荷
SecRule ARGS:data "@rx (eval|exec|system)" \
    "id:1,phase:2,t:base64Decode,deny,msg:'Base64 编码的代码执行检测'"

# 原始输入: ZXZhbChiYXNlNjRfZGVjb2RlKCR4KSk=
# 解码后: eval(base64_decode($x))

# 结合其他转换检测
SecRule ARGS "@detectSQLi" \
    "id:2,phase:2,t:base64Decode,t:urlDecode,deny,msg:'编码的 SQL 注入'"
```


**参数类型:** `无`


**区分大小写:** 是
