+++
title = "exec"
weight = 32
+++

**描述:** 执行外部脚本或命令

**语法:** `exec:path`

**区分大小写:** 是

**是否实现:** 否

Modsecurity中此 action 用来执行外部脚本/二进制文件。

此指令并未在WGE中实现，无法使用。

**示例:**

```apache
SecRule ARGS "@rx test" "id:1001,exec::/usr/local/apache/bin/test.lua,status:403"
```