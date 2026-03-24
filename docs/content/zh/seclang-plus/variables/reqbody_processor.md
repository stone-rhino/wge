+++
title = "REQBODY_PROCESSOR"
weight = 59
+++

**描述:** 请求体处理器类型

**语法:** `REQBODY_PROCESSOR`

REQBODY_PROCESSOR 包含当前请求使用的请求体处理器名称，如 URLENCODED、MULTIPART、JSON 或 XML。
当请求头中的 Content-Type 为`application/x-www-form-urlencoded`或者`multipart/form-data`时，WGE 会自动设置该变量为对应的URLENCODED或者MULTIPART，使用者也可通过 ctl:requestBodyProcessor 动作手动指定为JSON等。

**示例:**

```apache
# 根据处理器类型执行不同检查
SecRule REQUEST_HEADERS:Content-Type "^(?:application(?:/soap\+|/)|text/)xml" \
     "id:'200000',phase:1,t:none,t:lowercase,pass,nolog,ctl:requestBodyProcessor=XML"

SecRule REQUEST_HEADERS:Content-Type "^application/json" \
     "id:'200001',phase:1,t:none,t:lowercase,pass,nolog,ctl:requestBodyProcessor=JSON"
```
