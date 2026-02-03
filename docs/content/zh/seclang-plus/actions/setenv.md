+++
title = "setenv"
weight = 27
+++

**描述:** 设置环境变量

**信息:** Original Example: `setenv:attack_detected=1`

**语法:** `setenv | setenv:value`

**示例:**

```apache
SecRule ARGS "@rx test" "id:1001,setenv,status:403"
```


**参数类型:** `string`


**区分大小写:** 是
