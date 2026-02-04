+++
title = "@contains"
weight = 2
+++

**描述:** 包含指定字符串


**语法:** `"@contains string"`


@contains 检查变量值是否包含指定的子字符串。匹配区分大小写。相比 @rx，@contains 在进行简单子字符串查找时性能更优，因为不涉及正则表达式引擎开销。


**示例:**


```apache
# 检测 Content-Type 是否包含 multipart
SecRule REQUEST_HEADERS:Content-Type "@contains multipart" \
    "id:1068,phase:1,pass,nolog,ctl:requestBodyProcessor=MULTIPART"

# 检测 URL 中是否包含 admin
SecRule REQUEST_URI "@contains admin" \
    "id:1069,phase:1,log,msg:'访问管理路径'"
```


**参数类型:** `string`
