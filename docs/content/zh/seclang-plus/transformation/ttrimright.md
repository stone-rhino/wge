+++
title = "t:trimRight"
weight = 38
+++

**描述:** 对字符串去除结尾空白。

此转换函数会将输入字符串尾部的 ``\x20\t\n\r\f\v`` 字符去除。

**语法:** `t:trimRight`

**是否实现:** 是

**示例:**

```apache
# 去除参数结尾的空白后检测
SecRule ARGS:cmd "@rx ;$" \
    "id:1,phase:2,t:trimRight,deny,msg:'检测到命令分隔符'"

# 原始输入: "ls -la;   "
# 处理后: "ls -la;"
```
