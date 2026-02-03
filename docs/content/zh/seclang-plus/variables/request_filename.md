+++
title = "REQUEST_FILENAME"
weight = 17
+++

**描述:** 请求的文件路径


**语法:** `REQUEST_FILENAME`


REQUEST_FILENAME 包含请求 URI 中的文件路径部分，不包括查询字符串。这是从 URI 中提取的规范化路径，可用于检测对特定文件或目录的访问。


**示例:**


```apache
# 阻止访问管理目录
SecRule REQUEST_FILENAME "@beginsWith /admin" \
    "id:1024,phase:1,deny,msg:'禁止访问管理目录'"

# 阻止访问 PHP 配置文件
SecRule REQUEST_FILENAME "@endsWith .htaccess" \
    "id:1025,phase:1,deny,msg:'禁止访问 .htaccess'"
```
