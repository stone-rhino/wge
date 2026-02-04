+++
title = "SecDebugLog"
weight = 17
+++

**描述:** 配置调试日志文件路径。


**语法:** `SecDebugLog PATH`


**区分大小写:** 是



调试日志用于记录 WAF 规则引擎的详细运行信息，对于规则调试和问题排查非常有用。调试日志的详细程度由 SecDebugLogLevel 控制。建议在生产环境中仅在排查问题时启用调试日志。


**示例:**


```
SecDebugLog /var/log/wge_debug.log
```
