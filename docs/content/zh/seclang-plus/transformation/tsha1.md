+++
title = "t:sha1"
weight = 35
+++

**描述:** 计算 SHA1 哈希

**语法:** `t:sha1`

**示例:**

```apache
SecRule ARGS "@rx test" "id:1001,t:sha1"
```


**参数类型:** `string`


计算输入字符串的 SHA1 哈希值。计算结果为原始二进制格式，打印（或记录）时可能需要转码为文本。哈希函数常与十六进制编码组合使用（例如：t:sha1,t:hexEncode）。
