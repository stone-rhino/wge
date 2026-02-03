+++
title = "SecResponseBodyMimeType"
weight = 8
+++

**描述:** 配置需要检查的响应体 MIME 类型。

            
**语法:** `SecResponseBodyMimeType MIME_TYPE1 MIME_TYPE2 ...`

            
**区分大小写:** 是

            
            
可使用多个 SecResponseBodyMimeType 指令添加 MIME 类型。使用 SecResponseBodyMimeTypesClear 清除先前配置的 MIME 类型并重新开始。

            
**示例:**

            
```
SecResponseBodyMimeType text/plain text/html text/xml
```
