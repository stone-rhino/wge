+++
title = "logdata"
weight = 22
+++

**描述:** 记录额外数据到日志

**信息:** Original Example: `logdata:'User: %{MATCHED_VAR}'`

**语法:** `logdata | logdata:value`

**示例:**

```apache
SecRule ARGS "@rx test" "id:1001,logdata,status:403"
```


**参数类型:** `string`


**区分大小写:** 是
