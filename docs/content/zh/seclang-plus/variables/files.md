+++
title = "FILES"
weight = 75
+++

**描述:** 上传文件内容


**语法:** `FILES | FILES:fieldname`


FILES 包含通过 multipart/form-data 上传的文件的原始文件名。可通过 FILES:fieldname 访问特定表单字段中上传的文件。此变量是文件上传安全检查的核心，可用于验证文件扩展名、文件名格式等。


**示例:**


```apache
# 检查上传文件的危险扩展名
SecRule FILES "@rx \.(php|jsp|asp|exe|sh|bat)$" \
    "id:1050,phase:2,deny,status:403,msg:'禁止上传此类型文件'"
```
