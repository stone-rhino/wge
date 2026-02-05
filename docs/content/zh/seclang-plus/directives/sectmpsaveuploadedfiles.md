+++
title = "SecTmpSaveUploadedFiles"
weight = 32
+++

**描述:** 配置是否保存上传的文件到临时目录。


**语法:** `SecTmpSaveUploadedFiles On|Off`


**默认值:** Off


**区分大小写:** 是

**是否实现:** 否

启用后，通过 multipart/form-data 上传的文件将保存到 SecTmpDir 指定的临时目录。这允许规则使用 @inspectFile 操作符对上传文件进行深度检查（如病毒扫描）。

此指令WGE可以正常解析，但是暂未实现该功能。

**示例:**


```
SecTmpSaveUploadedFiles On
```
