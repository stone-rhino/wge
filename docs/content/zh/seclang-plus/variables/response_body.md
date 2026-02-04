+++
title = "RESPONSE_BODY"
weight = 24
+++

**描述:** 响应体内容


**语法:** `RESPONSE_BODY`


RESPONSE_BODY 包含服务器返回的响应体内容。需要启用 SecResponseBodyAccess 才能访问此变量。可用于检测响应中的敏感信息泄露、恶意内容或异常模式。在 phase:4 (响应体阶段) 中使用。


**示例:**


```apache
# 检测响应中的信用卡号泄露
SecRule RESPONSE_BODY "@rx \b(?:\d{4}[-\s]?){3}\d{4}\b" \
    "id:1034,phase:4,deny,msg:'检测到信用卡号泄露'"

# 检测 SQL 错误信息泄露
SecRule RESPONSE_BODY "@pm mysql_error ora-00 sql syntax error" \
    "id:1035,phase:4,deny,msg:'检测到数据库错误信息泄露'"
```
