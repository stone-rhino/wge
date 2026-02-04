+++
title = "SecAction"
weight = 38
+++

**描述:** 无条件执行指定的动作。


**语法:** `SecAction "ACTIONS"`


**版本:** 1.0



SecAction 用于无条件执行一组动作,常用于设置变量、初始化集合等。



**区分大小写:** 是



SecAction 无条件处理指定的动作列表，其参数语法与 SecRule 的第三参数完全一致。该指令常用于设置变量及通过 initcol 操作初始化持久集合。由于没有变量和操作符参数，SecAction 的动作将在每次处理时无条件执行。


**示例:**


```apache
SecAction "id:9001,phase:1,nolog,pass,setvar:tx.blocking_paranoia_level=2"
```
