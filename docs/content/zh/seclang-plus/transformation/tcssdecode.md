+++
title = "t:cssDecode"
weight = 15
+++

**描述:** 对字符串进行 ``CSS`` 转义序列还原。

**语法:** `t:cssDecode`

**是否实现:** 是

**示例:**

```apache
# 检测 CSS 注入攻击
SecRule ARGS "@rx expression|javascript" \
    "id:1,phase:2,t:cssDecode,t:lowercase,deny,msg:'CSS 注入检测'"

# 原始输入: \65\78\70\72\65\73\73\69\6f\6e (CSS 编码的 "expression")
# 解码后: expression
```
