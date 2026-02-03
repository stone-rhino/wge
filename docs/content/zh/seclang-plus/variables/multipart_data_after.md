+++
title = "MULTIPART_DATA_AFTER"
weight = 49
+++

**描述:** Multipart 边界后的数据


**语法:** `MULTIPART_DATA_AFTER`


**示例:**


```apache
SecRule MULTIPART_DATA_AFTER "@rx value" "id:1001,deny,msg:'Test'"
```


**是否实现:** 暂无
