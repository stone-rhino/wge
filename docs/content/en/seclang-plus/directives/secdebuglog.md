+++
title = "SecDebugLog"
weight = 17
+++

**Description:** Configure the debug log file path.

**Syntax:** `SecDebugLog PATH`

The debug log records detailed runtime information from the WAF rule engine and is very useful for rule debugging and troubleshooting. The verbosity level of the debug log is controlled by SecDebugLogLevel. It is recommended to enable debug logging in production environments only when troubleshooting issues.

**Example:**

```
SecDebugLog /var/log/wge_debug.log
```

**Case Sensitive:** Yes
