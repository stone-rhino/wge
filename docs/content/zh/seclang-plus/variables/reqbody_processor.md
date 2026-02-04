+++
title = "REQBODY_PROCESSOR"
weight = 59
+++

**描述:** 请求体处理器类型


**语法:** `REQBODY_PROCESSOR`


REQBODY_PROCESSOR 包含当前请求使用的请求体处理器名称，如 URLENCODED、MULTIPART、JSON 或 XML。此变量由 WGE 根据 Content-Type 自动设置，也可通过 ctl:requestBodyProcessor 动作手动指定。


**示例:**


```apache
# 根据处理器类型执行不同检查
SecRule REQBODY_PROCESSOR "@streq JSON" \
    "id:1046,phase:2,pass,nolog,setvar:tx.json_request=1"
```
