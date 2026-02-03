+++
title = "SecRequestBodyNoFilesLimit"
weight = 4
+++

**描述:** 配置不包含文件上传的请求体最大大小 (字节)。

            
**语法:** `SecRequestBodyNoFilesLimit LIMIT`

            
**默认值:** 131072 (128KB)

            
**区分大小写:** 是

            
            
通常而言，默认值不够小。对于大多数应用程序，您应能将其降低至 128 KB 或更低。超过限制的请求将返回状态码 413（请求实体过大）被拒绝。存在 1 GB 的硬性上限。

            
**示例:**

            
```
SecRequestBodyNoFilesLimit 131072
```
