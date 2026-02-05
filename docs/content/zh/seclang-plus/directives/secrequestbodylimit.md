+++
title = "SecRequestBodyLimit"
weight = 3
+++

**描述:** 配置可接受的最大请求体大小 (字节)。


**语法:** `SecRequestBodyLimit LIMIT`


**默认值:** 134217728 (128MB)


**区分大小写:** 是

**是否实现:** 否

超过限制的请求将返回状态码 413（请求实体过大）被拒绝。硬性上限为 1 GB。

此指令WGE可以正常解析，但是暂未实现该功能。

**示例:**


```apache
SecRequestBodyLimit 134217728
```
