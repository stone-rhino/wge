+++
title = "TX"
weight = 81
+++

**描述:** 事务变量集合 (临时变量)


**语法:** `TX:key`


TX (Transaction) 是最常用的集合变量，用于在单个事务（请求-响应周期）内存储和传递数据。TX 变量在事务结束后自动销毁。通过 setvar 动作设置，通过 TX:key 访问。TX:0 到 TX:9 为特殊变量，自动存储 @rx 操作符的正则捕获组结果。此外，TX 变量常用于实现异常评分模式（如 tx.anomaly_score）。


**示例:**


```apache
# 设置异常评分
SecRule ARGS "@detectSQLi" \
    "id:1052,phase:2,pass,setvar:tx.anomaly_score=+5,msg:'SQL 注入加分'"

# 在评分累计后进行阻断判断
SecRule TX:anomaly_score "@ge 10" \
    "id:1053,phase:2,deny,msg:'异常评分超过阈值: %{TX.anomaly_score}'"

# 使用正则捕获组
SecRule REQUEST_URI "@rx ^/user/(\d+)" \
    "id:1054,phase:1,pass,setvar:tx.user_id=%{TX.1}"
```
