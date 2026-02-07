+++
title = "@validateUrlEncoding"
weight = 22
+++

**描述:** 验证输入是否为 URL 编码格式

**语法:** `@validateUrlEncoding`

**输入数据类型:** `string`

@validateUrlEncoding 检查变量值中的 URL 编码（百分号编码）是否有效。如果发现例如 %ZZ 或 %1 无效的 URL 编码则匹配成功。

**注意：**WGE中会对部分内容进行默认解码操作，如ARGS、REQUEST_URI等，除非确定其是多次编码的，否则对齐使用操作符 validateUrlEncoding 几乎没有意义。

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