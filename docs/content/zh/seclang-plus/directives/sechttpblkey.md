+++
title = "SecHttpBlKey"
weight = 27
+++

**描述:** 配置 HTTP 黑名单 (Project Honeypot) API 密钥。


**语法:** `SecHttpBlKey API_KEY`


若 @rbl 操作符使用 dnsbl.httpbl.org RBL（http://www.projecthoneypot.org/httpbl_api.php），则必须提供 API 密钥。该密钥绑定至个人用户账户，并将包含在 RBL DNS 请求中。


**示例:**


```
SecHttpBlKey abcdefghijkl
```
