+++
title = "REQUEST_LINE"
weight = 20
+++

**描述:** 完整的请求行


**语法:** `REQUEST_LINE`


REQUEST_LINE 包含完整的 HTTP 请求行，格式为 "METHOD URI PROTOCOL"，例如 "GET /index.html HTTP/1.1"。可用于全面检查请求行中的异常内容。


**示例:**


```apache
# 检测请求行中的控制字符
SecRule REQUEST_LINE "@rx [\x00-\x08\x0b\x0c\x0e-\x1f]" \
    "id:1030,phase:1,deny,msg:'请求行包含控制字符'"

# 检测超长请求行
SecRule REQUEST_LINE "@gt 8192" "t:length,id:1031,phase:1,deny,msg:'请求行过长'"
```
