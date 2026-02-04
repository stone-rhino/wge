+++
title = "severity"
weight = 4
+++

**描述:** 严重级别 (0-7)

**语法:** `severity:CRITICAL|ERROR|WARNING|NOTICE|INFO|DEBUG`

**示例:**

```apache
SecRule ARGS "@rx test" "id:1001,deny,severity:CRITICAL"
```


**参数类型:** `int 或 string`


**区分大小写:** 是
