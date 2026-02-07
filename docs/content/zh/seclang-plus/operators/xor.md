+++
title = "@xor"
weight = 13
+++

**描述:** 数值相等


**语法:** `@xor string`

**输入数据类型:** `int`

@xor 对变量值进行异或操作，当异或结果非0时匹配成功。传入参数没有宏扩展时会和 eq 一样进行**前缀解析**。

**示例:**
```apache
# Content-Lenth和Transfer-Encoding不能同时存在
SecRule &REQUEST_HEADERS:Content-Lenth "!@xor %{&REQUEST_HEADERS:Transfer-Encoding}" \
    "id:1085,phase:3,pass,log,msg:'同时存在Content-Lenth头和Transfer-Encoding头'"
```