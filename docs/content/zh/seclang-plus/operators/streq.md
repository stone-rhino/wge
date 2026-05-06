+++
title = "@streq"
weight = 7
+++

**描述:** 字符串相等


**语法:** `@streq string`

**输入数据类型:** `string`

**区分大小写:** 是


@streq 检查变量值是否与指定的字符串完全相等。匹配区分大小写。相比 @rx "^exact$" 的正则表达式，@streq 在进行精确字符串比较时性能更优。


**示例:**
```apache
# 精确匹配 HTTP 方法
SecRule REQUEST_METHOD "@streq POST" \
    "id:1081,phase:1,pass,nolog,setvar:tx.is_post=1"

# 验证特定 Header 值
SecRule REQUEST_HEADERS:X-Requested-With "@streq XMLHttpRequest" \
    "id:1082,phase:1,pass,nolog,setvar:tx.is_ajax=1"

# 检测特定用户代理
SecRule REQUEST_HEADERS:User-Agent "@streq curl/7.68.0" \
    "id:1084,phase:1,log,pass,msg:'检测到 curl 客户端'"
```