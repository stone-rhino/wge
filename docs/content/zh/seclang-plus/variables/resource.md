+++
title = "RESOURCE"
weight = 86
+++

**描述:** 资源变量集合


**语法:** `RESOURCE:key`


RESOURCE 集合用于存储与特定资源（如 URL 路径）关联的持久化数据。可用于跟踪对特定资源的访问模式，如特定 API 端点的调用频率。


**示例:**


```apache
# 初始化资源集合
SecAction "id:1062,phase:1,pass,nolog,initcol:RESOURCE=%{REQUEST_FILENAME}"
```
