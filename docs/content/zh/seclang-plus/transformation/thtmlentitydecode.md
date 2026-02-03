+++
title = "t:htmlEntityDecode"
weight = 13
+++

**描述:** HTML 实体解码

**语法:** `t:htmlEntityDecode`

**示例:**

```apache
SecRule ARGS "@rx test" "id:1001,t:htmlEntityDecode"
```


**参数类型:** `string`


解码以 HTML 实体形式编码的字符。支持以下变体： 该函数始终将单个 HTML 实体转换为一个字节，可能导致信息丢失（若实体对应的字符无法用单字节表示）。因此它有助于揭示本无需编码的字节，但无法对 0xff 以上范围的字符进行有效处理。
