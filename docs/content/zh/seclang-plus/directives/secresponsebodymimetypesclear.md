+++
title = "SecResponseBodyMimeTypesClear"
weight = 23
+++

**描述:** 清除所有响应体 MIME 类型配置。

            
**语法:** `SecResponseBodyMimeTypesClear`

            
**区分大小写:** 是

            
            
此指令清除先前通过 SecResponseBodyMimeType 配置的所有 MIME 类型，允许从零开始重新定义需要缓冲的响应类型。通常用于覆盖继承的配置。

            
**示例:**

            
```
SecResponseBodyMimeTypesClear
SecResponseBodyMimeType text/html text/plain
```
