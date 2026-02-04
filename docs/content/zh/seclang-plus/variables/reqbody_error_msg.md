+++
title = "REQBODY_ERROR_MSG"
weight = 58
+++

**描述:** 请求体错误消息


**语法:** `REQBODY_ERROR_MSG`


REQBODY_ERROR_MSG 包含请求体解析失败时的错误描述信息。通常与 REQBODY_ERROR 配合使用，在日志中记录具体的错误原因，便于调试和分析。可通过 %{REQBODY_ERROR_MSG} 在 msg 动作中引用。


**示例:**


```apache
# 记录请求体解析错误的详细信息
SecRule REQBODY_ERROR "!@eq 0" \
    "id:1045,phase:2,pass,log,msg:'请求体错误: %{REQBODY_ERROR_MSG}'"
```
