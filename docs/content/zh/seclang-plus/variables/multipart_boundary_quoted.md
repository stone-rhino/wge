+++
title = "MULTIPART_BOUNDARY_QUOTED"
weight = 46
+++

**描述:** Multipart 边界引号错误


**语法:** `MULTIPART_BOUNDARY_QUOTED`


**示例:**


```apache
SecRule MULTIPART_BOUNDARY_QUOTED "@rx value" "id:1001,deny,msg:'Test'"
```


**是否实现:** 暂无
