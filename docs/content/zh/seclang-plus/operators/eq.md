+++
title = "@eq"
weight = 13
+++

**描述:** 数值相等


**语法:** `"@eq number"`


@eq 对变量值进行数值相等比较。变量值会先被转换为整数再进行比较，非数值字符串会被转换为 0。常用于检查状态码、计数值等精确数值匹配场景。支持使用宏扩展动态指定比较值。


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


**参数类型:** `int`


**区分大小写:** 是
