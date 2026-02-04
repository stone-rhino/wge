+++
title = "t:cssDecode"
weight = 15
+++

**描述:** CSS 解码


**语法:** `t:cssDecode`


t:cssDecode 解码 CSS 转义序列。CSS 允许使用反斜杠后跟十六进制数字（最多6位）来表示字符，攻击者可能利用此特性绕过检测。此转换函数可以规范化这些 CSS 编码的字符。


**示例:**


```apache
# 检测 CSS 注入攻击
SecRule ARGS "@rx expression|javascript" \
    "id:1,phase:2,t:cssDecode,t:lowercase,deny,msg:'CSS 注入检测'"

# 原始输入: \65\78\70\72\65\73\73\69\6f\6e (CSS 编码的 "expression")
# 解码后: expression
```


**参数类型:** `无`


**区分大小写:** 是
