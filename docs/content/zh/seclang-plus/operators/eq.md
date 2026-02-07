+++
title = "@eq"
weight = 13
+++

**描述:** 数值相等


**语法:** `@eq string`

**输入数据类型:** `int`

@eq 对变量值进行数值相等比较。当传入参数没有宏扩展时其会尝试将其转化为数字，其采用的是**前缀解析**的方式（如12abc解析为12，nothing2解析为0）；当传入参数宏扩展时需要保证其值是整数（不能是整数字符串）。

**示例:**


```apache
# 检测 404 响应状态
SecRule RESPONSE_STATUS "@eq 404" \
    "id:1085,phase:3,pass,log,msg:'页面未找到'"

# 检测空请求体
SecRule REQUEST_HEADERS:Content-Length "@eq 0" \
    "id:1086,phase:1,pass,nolog,setvar:tx.empty_body=1"

# 检测异常评分是否达到阈值
SecRule TX:anomaly_score "@eq %{TX.threshold}" \
    "id:1087,phase:2,deny,msg:'异常评分达到阈值'"
```