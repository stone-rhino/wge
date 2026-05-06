+++
title = "@within"
weight = 5
+++

**描述:** 检测输入字符串中是否存在子串与传入参数中任一字符串相同

**语法:** `@within string`

**输入数据类型:** `string`

within的实际实现与 pm 相同，所以详细信息可以见 pm 的说明。

**示例:**
```apache
# 验证 HTTP 方法是否在允许列表中
SecRule REQUEST_METHOD "!@within GET POST HEAD OPTIONS" \
    "id:1078,phase:1,deny,msg:'不允许的 HTTP 方法'"

# 验证文件扩展名是否在白名单中
SecRule FILES_COMBINED_SIZE "@gt 0" \
    "id:1079,phase:2,chain"
    SecRule FILES_NAMES "!@within .jpg .png .gif .pdf" \
        "deny,msg:'不允许的文件类型'"

# 验证 Content-Type 是否合法
SecRule REQUEST_HEADERS:Content-Type "!@within application/json application/xml text/plain" \
    "id:1080,phase:1,deny,msg:'不支持的 Content-Type'"
```