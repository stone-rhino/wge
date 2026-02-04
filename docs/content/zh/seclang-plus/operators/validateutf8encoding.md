+++
title = "@validateUtf8Encoding"
weight = 23
+++

**描述:** 验证 UTF-8 编码格式


**语法:** `"@validateUtf8Encoding parameter"`


**示例:**


```apache
SecRule ARGS "@validateUtf8Encoding parameter" "id:1001,deny,msg:'Test'"
```


**参数类型:** `无`


**是否实现:** 暂无
