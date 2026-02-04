+++
title = "PATH_INFO"
weight = 39
+++

**描述:** 路径信息


**语法:** `PATH_INFO`


PATH_INFO 包含 URL 中文件路径之后、查询字符串之前的附加路径信息。例如，对于 /cgi-bin/script.py/extra/path，PATH_INFO 的值为 /extra/path。在 RESTful API 中，此变量可能包含路由参数。


**示例:**


```apache
# 检测 PATH_INFO 中的目录遍历
SecRule PATH_INFO "@contains .." \
    "id:1040,phase:1,deny,msg:'PATH_INFO 检测到目录遍历'"
```
