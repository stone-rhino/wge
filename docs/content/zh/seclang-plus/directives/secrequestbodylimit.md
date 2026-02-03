+++
title = "SecRequestBodyLimit"
weight = 3
+++

**描述:** 配置可接受的最大请求体大小 (字节)。

            
**语法:** `SecRequestBodyLimit LIMIT`

            
**默认值:** 13107200 (12.5MB)

            
**区分大小写:** 是

            
            
超过限制的请求将返回状态码 413（请求实体过大）被拒绝。硬性上限为 1 GB。

            
**示例:**

            
```
SecRequestBodyLimit 13107200
```
