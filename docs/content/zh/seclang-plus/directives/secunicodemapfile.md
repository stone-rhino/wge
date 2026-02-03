+++
title = "SecUnicodeMapFile"
weight = 16
+++

**描述:** 配置 Unicode 映射文件路径和代码页。

            
**语法:** `SecUnicodeMapFile PATH CODEPAGE`

            
**区分大小写:** 是

            
            
该指令定义用于 urlDecodeUni 转换函数的 Unicode 映射文件路径。代码页参数指定目标字符编码（如 20127 表示 US-ASCII）。当处理使用非 ASCII 编码的 URL 时，此配置确保正确的字符转换。

            
**示例:**

            
```
SecUnicodeMapFile unicode.mapping 20127
```
