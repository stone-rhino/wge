+++
title = "t:none"
weight = 1
+++

**描述:** 用于清除转换链。

``t:none`` 用于清除转换链里位于 ``t:none`` 之前的所有转换函数。当使用 ``t:none`` 后，只有在其之后显式指定的转换函数才会被应用。

**语法:** `t:none`

**是否实现:** 是

**示例:**

```apache
# 假设默认动作包含 t:lowercase,t:urlDecode
SecDefaultAction "phase:2,log,pass,t:lowercase,t:urlDecode"

# 此规则会继承默认转换
SecRule ARGS "@rx admin" "id:1,deny,msg:'检测到 admin'"

# 使用 t:none 清除默认转换，仅使用 t:base64Decode
SecRule ARGS:encoded "@rx admin" \
    "id:2,t:none,t:base64Decode,deny,msg:'Base64 中检测到 admin'"

# 完全不进行任何转换
SecRule REQUEST_BODY "@rx ^\{" \
    "id:3,t:none,pass,setvar:tx.is_json=1"
```
