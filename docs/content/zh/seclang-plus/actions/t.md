+++
title = "t"
weight = 45
+++

**描述:** 应用转换函数

**语法:** `t:transformationName`

t（转换）动作在与操作符匹配之前对输入数据应用转换函数。转换可以规范化或解码数据以提高检测准确性。可以链接多个转换，按顺序应用。

常用转换：
- **t:none** - 禁用所有默认转换
- **t:lowercase** - 转换为小写
- **t:urlDecode** - URL 解码
- **t:htmlEntityDecode** - 解码 HTML 实体
- **t:base64Decode** - Base64 解码
- **t:removeWhitespace** - 移除所有空白字符
- **t:compressWhitespace** - 规范化空白字符
- **t:normalizePath** - 规范化路径分隔符
- **t:cmdLine** - 规范化命令行
- **t:hexDecode** - 解码十六进制编码数据

重要：使用 t:none 时，应将其指定为第一个转换，以清除从 SecDefaultAction 继承的转换。

**示例:**

```apache
# 使用 URL 解码和小写检测 SQL 注入
SecRule ARGS "@detectSQLi" \
    "id:100,phase:2,deny,t:urlDecode,t:lowercase,msg:'SQL 注入'"

# 清除默认转换后应用自定义转换
SecRule ARGS "@rx <script" \
    "id:101,phase:2,deny,t:none,t:htmlEntityDecode,t:lowercase,msg:'XSS'"

# 规范化路径以检测路径遍历
SecRule REQUEST_URI "@contains ../" \
    "id:102,phase:1,deny,t:normalizePath,msg:'路径遍历'"

# 链接多个解码以检测绕过尝试
SecRule ARGS "@rx eval\s*\(" \
    "id:103,phase:2,deny,t:urlDecode,t:base64Decode,t:lowercase,msg:'代码注入'"
```

**参数类型:** `string`

**区分大小写:** 是
