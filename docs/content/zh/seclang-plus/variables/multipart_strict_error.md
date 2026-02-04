+++
title = "MULTIPART_STRICT_ERROR"
weight = 44
+++

**描述:** Multipart 严格错误标志


**语法:** `MULTIPART_STRICT_ERROR`


MULTIPART_STRICT_ERROR 是一个复合标志变量，当 multipart 请求体解析过程中检测到任何严格模式违规时设置为 1。它综合了多个 multipart 错误标志（如 MULTIPART_UNMATCHED_BOUNDARY 等），用于快速判断 multipart 请求是否存在异常。这是防范文件上传绕过攻击的重要检查点。


**示例:**


```apache
# 拒绝存在 multipart 解析错误的请求
SecRule MULTIPART_STRICT_ERROR "!@eq 0" \
    "id:1042,phase:2,deny,status:403,msg:'Multipart 请求解析异常'"
```
