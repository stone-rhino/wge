+++
title = "INBOUND_DATA_ERROR"
weight = 61
+++

**描述:** 入站数据错误


**语法:** `INBOUND_DATA_ERROR`


**示例:**


```apache
SecRule INBOUND_DATA_ERROR "@rx value" "id:1001,deny,msg:'Test'"
```


**是否实现:** 暂无
