+++
title = "QUERY_STRING"
weight = 21
+++

**描述:** 查询字符串


**语法:** `QUERY_STRING`


QUERY_STRING 包含 URL 中问号 (?) 后面的查询字符串部分，不包括问号本身。这是原始的查询字符串，未经解析为单独的参数。可用于检测整体查询字符串的异常模式。


**示例:**


```apache
# 检测查询字符串中的 SQL 注入
SecRule QUERY_STRING "@detectSQLi" \
    "id:1032,phase:1,deny,msg:'查询字符串检测到 SQL 注入'"

# 限制查询字符串长度
SecRule QUERY_STRING "@gt 2048" "t:length,id:1033,phase:1,deny,msg:'查询字符串过长'"
```
