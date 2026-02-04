+++
title = "SecRequestBodyJsonDepthLimit"
weight = 6
+++

**描述:** 配置 JSON 对象的最大解析深度。


**语法:** `SecRequestBodyJsonDepthLimit DEPTH`


**默认值:** 512


**区分大小写:** 是



解析 JSON 对象时，若嵌套层级超过配置深度限制，解析将终止并设置 REQBODY_ERROR 错误。


**示例:**


```
SecRequestBodyJsonDepthLimit 512
```
