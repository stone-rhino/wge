+++
title = "t:base64Decode"
weight = 7
+++

**描述:** 对字符串进行 ``Base64`` 解码。

**语法:** `t:base64Decode`

**是否实现:** 是

**示例:**
```apache
# 检测 Base64 编码的攻击载荷
SecRule ARGS:data "@rx (eval|exec|system)" \
    "id:1,phase:2,t:base64Decode,deny,msg:'Base64 编码的代码执行检测'"

# 原始输入: ZXZhbChiYXNlNjRfZGVjb2RlKCR4KSk=
# 解码后: eval(base64_decode($x))
```
