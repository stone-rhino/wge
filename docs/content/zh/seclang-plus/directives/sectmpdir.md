+++
title = "SecTmpDir"
weight = 14
+++

**描述:** 配置临时文件存储目录。

            
**语法:** `SecTmpDir PATH`

            
**区分大小写:** 是

            
            
此目录用于存储请求体处理过程中的临时文件，特别是当请求体大小超过 SecRequestBodyInMemoryLimit 设置时，WAF 将把超出部分写入此目录。该目录需对运行 WAF 进程的用户具有可写权限。

            
**示例:**

            
```
SecTmpDir /tmp/
```
