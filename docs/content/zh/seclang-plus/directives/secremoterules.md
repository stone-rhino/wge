+++
title = "SecRemoteRules"
weight = 28
+++

**描述:** 从远程 HTTPS 服务器加载规则配置。


**区分大小写:** 是


**是否实现:** 否


此指令允许从远程服务器动态加载规则，便于集中管理和更新安全策略。KEY 参数用于身份验证，目标服务器可据此为不同客户端提供差异化规则集。


**示例:**


```apache
SecRemoteRules some-key https://example.com/rules.conf
```
