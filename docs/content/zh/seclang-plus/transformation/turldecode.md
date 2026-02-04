+++
title = "t:urlDecode"
weight = 4
+++

**描述:** URL 解码

**语法:** `t:urlDecode`

**示例:**

```apache
SecRule ARGS "@rx test" "id:1001,t:urlDecode"
```


**参数类型:** `string`


解码 URL 编码的输入字符串。无效编码（即使用非十六进制字符的编码，或位于字符串末尾且缺失一至两个字节的编码）不会被转换，但不会抛出错误。要检测无效编码，请先对输入数据使用 @validateUrlEncoding 操作符。请勿对已进行 URL 解码的变量（如请求参数）使用此转换函数，除非您有意执行两次 URL 解码！
