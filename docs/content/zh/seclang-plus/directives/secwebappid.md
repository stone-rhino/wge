+++
title = "SecWebAppId"
weight = 30
+++

**描述:** 配置 Web 应用程序标识符,用于区分不同的应用程序。


**语法:** `SecWebAppId NAME`


**区分大小写:** 是



应用程序命名空间用于避免同一服务器部署多个应用时会话 ID 与用户 ID 发生冲突。若未使用该功能，会话 ID 可能发生冲突。
在上述两个示例配置中，SecWebAppId 与 nginx 服务器块协同使用。配置值将存储于 WEBAPPID 变量中。


**示例:**


```
SecWebAppId "MyWebApp"
```
