+++
title = "OUTBOUND_DATA_ERROR"
weight = 62
+++

**描述:** 出站数据错误


**语法:** `OUTBOUND_DATA_ERROR`


**示例:**


```apache
SecRule OUTBOUND_DATA_ERROR "@rx value" "id:1001,deny,msg:'Test'"
```


**是否实现:** 暂无
