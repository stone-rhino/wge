+++
title = "REQUEST_URI_RAW"
weight = 16
+++

**描述:** 原始请求 URI (未解码)


**语法:** `REQUEST_URI_RAW`


REQUEST_URI_RAW 包含原始的、未经 URL 解码的请求 URI。用于检测编码绕过攻击，攻击者可能使用多重编码或异常编码来绕过 WAF 检测。建议同时检查 REQUEST_URI 和 REQUEST_URI_RAW 以获得更全面的保护。


**示例:**


```apache
# 检测 URL 中的双重编码攻击
SecRule REQUEST_URI_RAW "@rx %25" \
    "id:1022,phase:1,deny,msg:'检测到双重编码'"

# 检测 NULL 字节注入
SecRule REQUEST_URI_RAW "@rx %00" \
    "id:1023,phase:1,deny,msg:'检测到 NULL 字节注入'"
```
