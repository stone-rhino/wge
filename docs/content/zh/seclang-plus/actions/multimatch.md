+++
title = "multiMatch"
weight = 24
+++

**描述:** 对每次转换函数生成的结果都执行匹配。

**语法:** `multiMatch`

**参数类型:** `无`

**区分大小写:** 是

每条规则几乎都存在多个转换函数，当不存在动作 multiMatch 时，输入字符串会经过所有转换函数再进行匹配，而加上动作 multiMatch 后，每次转换函数执行后生成的字符串都会尝试匹配。

**示例:**

```apache
SecRule ARGS "@rx test" "id:1001,t:none,t:removeNulls,t:lowercase,multiMatch,status:403"
```