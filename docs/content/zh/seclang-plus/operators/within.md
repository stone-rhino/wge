+++
title = "@within"
weight = 5
+++

**描述:** 值在指定字符串中


**语法:** `"@within string"`


@within 是 @contains 的反向操作：检查变量值是否作为子字符串存在于指定的参数字符串中。换句话说，它检查参数字符串是否包含变量值。匹配区分大小写。这在创建白名单检查时特别有用，可以验证值是否在允许的列表中。


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


**参数类型:** `string`


**区分大小写:** 是
