+++
title = "t:trimLeft"
weight = 37
+++

**描述:** 对字符串去除开头空白。

此转换函数会将输入字符串首部的 ``\x20\t\n\r\f\v`` 字符去除。

**语法:** `t:trimLeft`

**是否实现:** 是

**示例:**

```apache
# 去除参数开头的空白后检测
SecRule ARGS:username "@rx ^admin" \
    "id:1,phase:2,t:trimLeft,deny,msg:'检测到 admin 用户名'"

# 原始输入: "   admin"
# 处理后: "admin"
```


