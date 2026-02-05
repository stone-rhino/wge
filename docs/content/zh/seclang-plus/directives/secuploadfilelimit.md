+++
title = "SecUploadFileLimit"
weight = 34
+++

**描述:** 配置单次请求允许上传的最大文件数量。


**语法:** `SecUploadFileLimit INT`


**默认值:** 0


**区分大小写:** 是

当上传的文件数量超过设置的值时，会设置错误标志 MULTIPART_FILE_LIMIT_EXCEEDED，该错误值和其它multipart/form-data格式的错误被包含在变量 MULTIPART_STRICT_ERROR 中。

**示例:**


```apache
SecUploadFileLimit 10
```
