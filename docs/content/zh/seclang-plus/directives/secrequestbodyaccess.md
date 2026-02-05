+++
title = "SecRequestBodyAccess"
weight = 2
+++

**描述:** 配置是否允许 WAF 访问请求体内容。


**语法:** `SecRequestBodyAccess On|Off`


**默认值:** Off


**区分大小写:** 是



若需检查请求主体传输的数据（如 POST 参数），则必须使用此指令。为实现可靠阻塞，请求缓冲亦不可或缺。可选值：
- **On:** 缓冲并检查请求体
- **Off:** 不缓冲请求体

这个命令实际上并不影响WGE是否缓冲请求体，因为缓冲这个动作WGE并不能控制，其只能被动接收来自服务器的请求体，如果需要此命令生效，需要在服务器中增加相关逻辑。

**示例:**


```apache
SecRequestBodyAccess On
```
