+++
title = "SecRemoteRules"
weight = 28
+++

**描述:** 从远程 HTTPS 服务器加载规则配置。


**语法:** `SecRemoteRules KEY URL`


**区分大小写:** 是



此指令允许从远程服务器动态加载规则，便于集中管理和更新安全策略。KEY 参数用于身份验证，目标服务器可据此为不同客户端提供差异化规则集。





**示例:**


```
SecRemoteRules some-key https://example.com/rules.conf
```
