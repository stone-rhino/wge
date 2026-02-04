+++
title = "SecArgumentSeparator"
weight = 11
+++

**描述:** 指定应用于 application/x-www-form-urlencoded 内容的参数分隔符字符。


**语法:** `SecArgumentSeparator CHARACTER`


**默认值:** &


**区分大小写:** 是



当后端 Web 应用程序使用非标准参数分隔符时需要此指令。某些应用程序会采用分号作为分隔符。除非确认所处理的应用程序需要不同分隔符，否则不应更改默认设置。若未为每个 Web 应用程序正确配置此指令，将无法正确解析参数，导致规则匹配效果显著降低。


**示例:**


```
SecArgumentSeparator &
```
