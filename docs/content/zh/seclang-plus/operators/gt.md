+++
title = "@gt"
weight = 14
+++

**描述:** 大于


**语法:** `"@gt number"`


@gt 检查变量值（转换为整数后）是否大于指定的数值。常用于检测超限情况，如请求大小超限、频率超限、评分超标等场景。是实现限流和阈值检测的核心操作符之一。


**示例:**


```apache
# 检测请求体过大 (超过 10MB)
SecRule REQUEST_HEADERS:Content-Length "@gt 10485760" \
    "id:1088,phase:1,deny,msg:'请求体超过大小限制'"

# 检测异常评分超标
SecRule TX:anomaly_score "@gt 10" \
    "id:1089,phase:2,deny,msg:'异常评分超过阈值: %{TX.anomaly_score}'"

# 检测 IP 请求频率超限
SecRule IP:request_count "@gt 100" \
    "id:1090,phase:1,deny,msg:'IP 请求频率超限'"

# 检测参数数量异常
SecRule &ARGS "@gt 100" \
    "id:1091,phase:2,deny,msg:'参数数量异常'"
```


**参数类型:** `int`


**区分大小写:** 是
