+++
title = "SecUploadDir"
weight = 33
+++

**描述:** 配置文件上传的存储目录。


**语法:** `SecUploadDir PATH`


**区分大小写:** 是



该目录必须与 SecTmpDir 定义的临时目录位于同一文件系统。此指令需配合 SecUploadKeepFiles 使用。


**示例:**


```
SecUploadDir /var/lib/wge/upload/
```
