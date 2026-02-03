+++
title = "t:none"
weight = 1
+++

**描述:** 清除转换链


**语法:** `t:none`


t:none 用于清除之前所有的转换函数，重置转换链。这在需要覆盖 SecDefaultAction 或 SecRule 继承的默认转换时非常有用。当使用 t:none 后，只有在其之后显式指定的转换函数才会被应用。


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


**参数类型:** `无`


**区分大小写:** 是
