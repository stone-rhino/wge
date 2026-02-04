+++
title = "exec"
weight = 32
+++

**描述:** 执行外部脚本或命令

**信息:** Original Example: `exec:/path/to/script.sh`

**语法:** `exec | exec:value`

**示例:**

```apache
SecRule ARGS "@rx test" "id:1001,exec,status:403"
```


**参数类型:** `string (文件路径)`


**区分大小写:** 是
