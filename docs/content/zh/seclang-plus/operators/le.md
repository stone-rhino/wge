+++
title = "@le"
weight = 17
+++

**描述:** 小于等于


**语法:** `@le string`

**输入数据类型:** `int`

@le 检查输入数据是否小于等于指定的数值。常用于上限检测，与 @ge 配合可实现闭区间范围检测。

**示例:**


```apache
# 规则触发总分数在指定范围内，仅记录日志
SecRule TX:defend_level "@le 20" "id:1001,phase:4,t:none,chain,logdata:'the %{REMOTE_ADDR}' defend_level is %{tx.defend_level}"
  SecRule TX:defend_level "@ge 10" "t:none"
```