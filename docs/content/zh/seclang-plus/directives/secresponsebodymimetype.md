+++
title = "SecResponseBodyMimeType"
weight = 8
+++

**描述:** 配置需要检查的响应体 MIME 类型。


**语法:** `SecResponseBodyMimeType MIME_TYPE1 MIME_TYPE2 ...`


**区分大小写:** 是

**是否实现:** 否

此指令WGE可以正常解析，但是暂未实现该功能。

如果需要此功能，可以通过content-type和skip/skipafter控制对于特定MIME是否执行规则。

或者在服务器中读取WGE中解析的配置，通过此判断是否传入Body。

当此指令多次出现时，最后解析的结果是所有指令的合并。

**示例:**


```apache
SecResponseBodyMimeType text/plain text/html text/xml
```
