+++
title = "@validateUrlEncoding"
weight = 22
+++

**描述:** 验证 URL 编码格式


**语法:** `"@validateUrlEncoding"`


@validateUrlEncoding 检查变量值中的 URL 编码（百分号编码）是否有效。如果发现无效的 URL 编码则匹配成功。此操作符用于检测利用无效 URL 编码进行规避检测的攻击，例如 %ZZ 或 %1 这样的格式错误编码。不需要参数。


**示例:**


```apache
# 检测无效的 URL 编码
SecRule ARGS "@validateUrlEncoding" \
    "id:1113,phase:2,deny,msg:'检测到无效的 URL 编码'"

# 检测 URI 中的无效编码
SecRule REQUEST_URI "@validateUrlEncoding" \
    "id:1114,phase:1,deny,msg:'URI 中存在无效的 URL 编码'"

# 检测查询字符串中的编码问题
SecRule QUERY_STRING "@validateUrlEncoding" \
    "id:1115,phase:1,deny,msg:'查询字符串中存在无效编码'"
```


**参数类型:** `无`


**区分大小写:** 是
