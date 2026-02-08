+++
title = "t"
weight = 45
+++

**描述:** 应用转换函数

**语法:** `t:transformationName`

**区分大小写:** 是

t（转换）动作在与操作符匹配之前对输入数据应用转换函数。转换用于规范化或解码数据以提高检测准确性。可以链式使用多个转换函数，按顺序依次应用。

常用转换函数：
- **t:none** - 清除所有默认转换
- **t:lowercase** - 转换为小写
- **t:urlDecode** - URL 解码
- **t:htmlEntityDecode** - HTML 实体解码
- **t:base64Decode** - Base64 解码
- **t:removeWhitespace** - 移除所有空白字符
- **t:compressWhitespace** - 规范化空白字符
- **t:normalizePath** - 规范化路径分隔符
- **t:cmdLine** - 规范化命令行
- **t:hexDecode** - 十六进制解码

注意：使用 t:none 时，应将其作为第一个转换函数，以清除从 SecDefaultAction 继承的默认转换。

**示例:**

```apache
# 使用 URL 解码和小写转换检测 SQL 注入
SecRule ARGS "@detectSQLi" \
    "id:100,phase:2,deny,t:urlDecode,t:lowercase,msg:'SQL Injection'"

# 清除默认转换后应用自定义转换
SecRule ARGS "@rx <script" \
    "id:101,phase:2,deny,t:none,t:htmlEntityDecode,t:lowercase,msg:'XSS'"

# 规范化路径以检测路径遍历
SecRule REQUEST_URI "@contains ../" \
    "id:102,phase:1,deny,t:normalizePath,msg:'Path Traversal'"

# 链式多重解码以检测编码绕过
SecRule ARGS "@rx eval\s*\(" \
    "id:103,phase:2,deny,t:urlDecode,t:base64Decode,t:lowercase,msg:'Code Injection'"
```
