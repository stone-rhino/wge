+++
title = "REQBODY_ERROR"
weight = 57
+++

**描述:** 请求体错误标志


**语法:** `REQBODY_ERROR`


REQBODY_ERROR 当请求体解析过程中出现错误时设置为 1。例如，当 JSON 格式无效、XML 格式错误或 multipart 数据异常时，此变量会被设置。可用于在 phase:2 中检测并阻止包含无效请求体的请求。


**示例:**


```apache
# 拒绝请求体解析失败的请求
SecRule REQBODY_ERROR "!@eq 0" \
    "id:1044,phase:2,deny,status:400,msg:'请求体解析失败: %{REQBODY_ERROR_MSG}'"
```
