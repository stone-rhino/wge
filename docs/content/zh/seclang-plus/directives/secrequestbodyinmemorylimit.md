+++
title = "SecRequestBodyInMemoryLimit"
weight = 22
+++

**描述:** 配置请求体在内存中缓冲的最大大小，超过此限制将写入临时文件。

            
**语法:** `SecRequestBodyInMemoryLimit LIMIT`

            
**默认值:** 131072 (128KB)

            
**区分大小写:** 是

            
            
此指令控制 WAF 将多少请求体数据保留在内存中。当请求体大小超过此限制时，超出部分将写入 SecTmpDir 指定的临时目录。适当设置此值可在内存使用和 I/O 性能之间取得平衡。

            
**示例:**

            
```
SecRequestBodyInMemoryLimit 131072
```
