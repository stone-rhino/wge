+++
title = "REQUEST_BODY"
weight = 12
+++

**描述:** 请求体内容


**语法:** `REQUEST_BODY`


REQUEST_BODY 包含原始的请求体数据。与 ARGS_POST 不同，REQUEST_BODY 是未解析的原始数据。当请求体处理器无法解析内容时（如未知的 Content-Type），可以使用此变量检查原始数据。需要启用 SecRequestBodyAccess 才能访问此变量。


**示例:**


```apache
# 检查原始请求体中的恶意模式
SecRule REQUEST_BODY "@rx
