+++
title = "XML"
weight = 73
+++

**描述:** XML 数据


**语法:** `XML:/xpath/expression`


XML 变量用于通过 XPath 表达式访问 XML 请求体中的特定节点。需要先通过 ctl:requestBodyProcessor=XML 启用 XML 解析器。支持完整的 XPath 语法，包括属性访问、文本节点提取等。


**示例:**


```apache
# 启用 XML 解析
SecRule REQUEST_HEADERS:Content-Type "@contains xml" \
    "id:1048,phase:1,pass,nolog,ctl:requestBodyProcessor=XML"

# 使用 XPath 检查 XML 节点
SecRule XML:/root/user/name/text() "@detectSQLi" \
    "id:1049,phase:2,deny,msg:'XML 节点检测到 SQL 注入'"
```
