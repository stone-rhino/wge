+++
title = "setenv"
weight = 27
+++

**描述:** 设置Linux/Unix环境变量，如果该变量存在则覆盖。

**信息:** Original Example: `setenv:attack_detected=1`

**语法:** `setenv:'string=(value|%{variable})'`

**区分大小写:** 是

此动作支持不匹配时执行和无论是否匹配都执行。

**示例:**

```apache
SecRule ARGS "@rx test" "id:1001,setenv:http_proxy=%{REMOTE_ADDR},status:403"

# 匹配失败后执行
SecRule ARGS "@rx test" "id:1002,!setenv:http_proxy=%{REMOTE_ADDR},status:403"

# 无论是否匹配成功都执行
SecRule ARGS "@rx test" "id:1003,*setenv:http_proxy=%{REMOTE_ADDR},status:403"
```