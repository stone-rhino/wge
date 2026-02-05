+++
title = "SecRequestBodyJsonDepthLimit"
weight = 6
+++

**描述:** 配置 JSON 对象的最大解析深度。


**语法:** `SecRequestBodyJsonDepthLimit DEPTH`


**默认值:** 0（unlimited）


**区分大小写:** 是

**是否实现:** 否


解析 JSON 对象时，若嵌套层级超过配置深度限制，解析将终止并设置 REQBODY_ERROR 错误。

此指令WGE可以正常解析，但是暂未实现该功能。

**示例:**


```apache
SecRequestBodyJsonDepthLimit 512
```
