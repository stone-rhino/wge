+++
title = "REQBODY_PROCESSOR_ERROR"
weight = 60
+++

**描述:** 请求体处理器错误


**语法:** `REQBODY_PROCESSOR_ERROR`


**示例:**


```apache
SecRule REQBODY_PROCESSOR_ERROR "@rx value" "id:1001,deny,msg:'Test'"
```


**是否实现:** 暂无
