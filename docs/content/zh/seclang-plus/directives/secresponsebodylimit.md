+++
title = "SecResponseBodyLimit"
weight = 9
+++

**描述:** 配置响应体缓冲区的最大大小 (字节)。

            
**语法:** `SecResponseBodyLimit LIMIT`

            
**默认值:** 524288 (512KB)

            
**区分大小写:** 是

            
            
超过此限制的响应将被拒绝。此设置不影响未选中缓冲的 MIME 类型的响应。存在 1 GB 的硬性限制。

            
**示例:**

            
```
SecResponseBodyLimit 524288
```
