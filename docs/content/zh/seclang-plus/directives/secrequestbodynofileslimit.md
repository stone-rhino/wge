+++
title = "SecRequestBodyNoFilesLimit"
weight = 4
+++

**描述:** 配置不包含文件上传的请求体最大大小 (字节)。


**语法:** `SecRequestBodyNoFilesLimit LIMIT`


**默认值:** 1048576 (1MB)


**区分大小写:** 是

**是否实现:** 否

此指令WGE可以正常解析，但是暂未实现该功能。


**示例:**


```apache
SecRequestBodyNoFilesLimit 131072
```
