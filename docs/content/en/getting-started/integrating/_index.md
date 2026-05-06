+++
title = "Integrating"
weight = 3
+++

## Overview

WGE is a standalone security inspection engine that does not handle network traffic directly.

By integrating WGE into a web server (such as Nginx, Apache, or Envoy), you can introduce security inspection into the request processing pipeline, enabling real-time analysis and interception of HTTP traffic.

This integration approach offers the following advantages:

- Unified security enforcement at the traffic entry point (WAF capability)
- Seamless combination with existing reverse proxies or gateways
- Reuse of the web server's logging and operations infrastructure

Depending on your deployment environment, WGE supports multiple integration methods:

- [Nginx](./integrate-into-nginx.md)
- [Apache](./integrate-into-apache.md)
- [Envoy](./integrate-into-envoy.md)

Select the appropriate integration method for your environment.

---

## Repository

All WGE web server connector code is maintained in the [wge-connectors](https://github.com/stone-rhino/wge-connectors) repository. You can build directly using the provided [build.sh](https://github.com/stone-rhino/wge-connectors/blob/main/build.sh) script.

---

## Logging

When WGE runs inside a web server, it does not write its own log files. Instead, log output is redirected to the host server's logging system and managed by the host server.

All WGE log entries carry a unified prefix **WGE:** to distinguish them from the server's own logs.

WGE log levels in ascending order are: `trace` → `debug` → `info` → `warn` → `error` → `critical` → `off`. These levels are mapped to the corresponding log levels of the host server. The handling of the `trace` level requires special attention:

- **Apache**: WGE `trace` maps to Apache's `APLOG_TRACE1`
- **Nginx**: Because Nginx does not support a `trace` level, WGE `trace` maps to `debug`
- **Envoy**: Envoy dynamic modules do not emit `trace` logs; WGE `trace` maps to `debug`

**⚠️ Note**: In Nginx and Envoy, because `trace` is mapped to `debug`, enabling the `debug` log level may generate a large volume of WGE log output. Enable it only when debugging.

---

### Limitations

The logging mechanism described above applies only to WGE's own logs, not to those of its dependencies.

For example, when the regex library re2 (used by WGE) encounters a compilation failure, it may write directly to standard error (STDERR). Such log entries:

- Do not carry the `WGE:` prefix
- Are not subject to WGE log level control
- Appear directly in the server's error log
