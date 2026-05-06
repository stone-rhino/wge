+++
title = "集成"
weight = 3
+++

## 概述

WGE 是一个独立的安全检测引擎，本身不直接处理网络流量。

通过将 WGE 集成到 Web Server（如 Nginx、Apache、Envoy）中，可以在请求处理链路中引入安全检测能力，实现对 HTTP 请求的实时分析与拦截。

这种集成方式具有以下优势：

- 在流量入口统一进行安全防护（WAF 能力）
- 与现有反向代理或网关无缝结合
- 复用 Web Server 的日志与运维体系

根据不同的部署环境，WGE 提供了多种集成方式：

- [Nginx](./integrate-into-nginx.md)
- [Apache](./integrate-into-apache.md)
- [Envoy](./integrate-into-envoy.md)

请选择对应的集成方式查看具体配置说明。

---

## 仓库地址
WGE 各类 Web Server Connector 代码统一维护在 [wge-connectors](https://github.com/stone-rhino/wge-connectors) 仓库中，可以直接使用仓库的脚本文件[build.sh](https://github.com/stone-rhino/wge-connectors/blob/main/build.sh)构建。

---


## 日志

WGE 在各类 Web Server 中运行时，不会单独输出日志文件，而是将日志重定向到宿主服务器的日志系统中，由宿主服务器统一管理。

所有 WGE 日志均带有统一前缀：**WGE:**，用于与服务器自身日志进行区分。

WGE 的日志等级从低到高为： trace → debug → info → warn → error → critical → off 这些日志级别会映射到宿主服务器对应的日志级别，其中关于 `trace` 级别的处理需要特别注意：

- **Apache**：WGE 的 `trace` 日志映射为 Apache 的 `APLOG_TRACE1`
- **Nginx**：由于 Nginx 不支持 `trace` 级别，WGE 的 `trace` 日志会映射为 `debug`
- **Envoy**：Envoy 动态模块不会输出 `trace` 日志，WGE 的 `trace` 会映射为 `debug`

**⚠️ 注意**：在 Nginx 和 Envoy 中，由于 `trace` 被映射为 `debug`，当开启 `debug` 日志级别时，可能会产生大量 WGE 日志输出，建议仅在调试场景下启用。

---

### 局限性

上述日志机制仅适用于 WGE 本身的日志，不包括其依赖库的日志。

例如对于 WGE 使用的正则库 re2，当正则编译失败时，可能会直接将日志输出到标准错误（STDERR）。此类日志：

- 不带有 `WGE:` 前缀
- 不受 WGE 日志级别控制
- 会直接出现在服务器的错误日志中