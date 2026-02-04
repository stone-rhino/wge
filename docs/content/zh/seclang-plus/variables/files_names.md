+++
title = "FILES_NAMES"
weight = 76
+++

**描述:** 上传文件名称


**语法:** `FILES_NAMES`


FILES_NAMES 包含所有上传文件对应的表单字段名称。可用于检测表单字段名中的异常内容，或验证上传文件是否通过预期的表单字段提交。


**示例:**


```apache
# 确保文件仅通过 'avatar' 字段上传
SecRule FILES_NAMES "!@streq avatar" \
    "id:1051,phase:2,deny,msg:'非法的文件上传字段'"
```
