+++
title = "MULTIPART_PART_HEADERS"
weight = 42
+++

**描述:** Multipart 部分头


**语法:** `MULTIPART_PART_HEADERS | MULTIPART_PART_HEADERS:HeaderName`


MULTIPART_PART_HEADERS 包含 multipart/form-data 请求中各部分的头信息。可用于检测上传文件部分的 Content-Type、Content-Disposition 等头信息中的异常内容。


**示例:**


```apache
# 检测 multipart 头中的异常 Content-Type
SecRule MULTIPART_PART_HEADERS "@rx application/(php|x-php|x-httpd-php)" \
    "id:1041,phase:2,deny,msg:'上传文件 Content-Type 异常'"
```
