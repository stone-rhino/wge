+++
title = "REQUEST_URI"
weight = 15
+++

**描述:** 请求 URI (已解码)


**语法:** `REQUEST_URI`


REQUEST_URI 包含完整的请求 URI，包括查询字符串部分，且已经过 URL 解码。这是检测 URL 中攻击载荷的主要变量。与 REQUEST_URI_RAW 不同，此变量的内容已解码，便于直接匹配攻击模式。


**示例:**


```apache
# 检测 URL 中的目录遍历攻击
SecRule REQUEST_URI "@rx \.\." \
    "id:1020,phase:1,deny,msg:'检测到目录遍历攻击'"

# 阻止访问敏感文件
SecRule REQUEST_URI "@rx \.(conf|ini|log|bak)$" \
    "id:1021,phase:1,deny,msg:'尝试访问敏感文件'"
```
