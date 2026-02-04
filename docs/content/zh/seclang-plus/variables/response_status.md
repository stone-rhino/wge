+++
title = "RESPONSE_STATUS"
weight = 27
+++

**描述:** 响应状态码


**语法:** `RESPONSE_STATUS`


RESPONSE_STATUS 包含服务器返回的 HTTP 状态码，如 200、404、500 等。可用于检测异常响应或基于状态码的安全策略。在 phase:3 (响应头阶段) 或之后可用。


**示例:**


```apache
# 记录所有 5xx 错误
SecRule RESPONSE_STATUS "@rx ^5" \
    "id:1036,phase:3,pass,log,msg:'服务器错误: %{RESPONSE_STATUS}'"

# 检测信息泄露（某些应用返回 500 可能包含调试信息）
SecRule RESPONSE_STATUS "@eq 500" \
    "id:1037,phase:3,pass,log,msg:'检测到服务器内部错误'"
```
