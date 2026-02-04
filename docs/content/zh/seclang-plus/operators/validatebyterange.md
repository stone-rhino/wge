+++
title = "@validateByteRange"
weight = 21
+++

**描述:** 验证字节范围


**语法:** `"@validateByteRange range1,range2-range3,..."`


@validateByteRange 检查变量值中的每个字节是否在指定的范围内。支持单个值（如 10）和范围（如 32-126）的组合，用逗号分隔。如果发现范围外的字节则匹配成功。此操作符最常用于检测 NUL 字节（0x00）的存在——这类字节虽无正当用途，却常被用作规避检测的技术。也可用于检测非打印字符和二进制数据。


**示例:**


```apache
# 检测 NUL 字节 (常见的规避技术)
SecRule ARGS "@validateByteRange 1-255" \
    "id:1109,phase:2,deny,msg:'检测到 NUL 字节注入'"

# 仅允许可打印 ASCII 字符 (32-126) 和常见控制字符
SecRule ARGS "@validateByteRange 9,10,13,32-126" \
    "id:1110,phase:2,deny,msg:'检测到非法字符'"

# 检测参数中的二进制数据
SecRule ARGS:data "!@validateByteRange 32-126" \
    "id:1111,phase:2,pass,log,msg:'参数包含非打印字符'"

# 在 URL 中检测非法字符
SecRule REQUEST_URI "@validateByteRange 1-255" \
    "id:1112,phase:1,deny,msg:'URL 中检测到 NUL 字节'"
```


**参数类型:** `string`


**区分大小写:** 是
