+++
title = "MULTIPART_UNMATCHED_BOUNDARY"
weight = 45
+++

**描述:** Multipart 不匹配的边界


**语法:** `MULTIPART_UNMATCHED_BOUNDARY`


MULTIPART_UNMATCHED_BOUNDARY 当 multipart 请求体中出现未匹配的边界标记时设置为 1。不匹配的边界通常表明请求被篡改或构造异常，可能是攻击者尝试绕过上传文件检测的手段。


**示例:**


```apache
# 拒绝包含不匹配边界的 multipart 请求
SecRule MULTIPART_UNMATCHED_BOUNDARY "!@eq 0" \
    "id:1043,phase:2,deny,status:403,msg:'Multipart 边界不匹配'"
```
