+++
title = "SecUploadKeepFiles"
weight = 36
+++

**描述:** 配置是否在请求处理完成后保留上传的文件。

            
**语法:** `SecUploadKeepFiles On|Off|RelevantOnly`

            
**默认值:** Off

            
**可选值:**

            
- **On:** 保留所有上传文件
- **Off:** 不保留上传文件
- **RelevantOnly:** 仅保留触发规则的请求中的上传文件
**区分大小写:** 是

            
            
此指令要求存储目录已通过 SecUploadDir 定义。

            
**示例:**

            
```
SecUploadKeepFiles RelevantOnly
```
