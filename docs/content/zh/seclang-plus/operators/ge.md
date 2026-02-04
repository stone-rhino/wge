+++
title = "@ge"
weight = 16
+++

**描述:** 大于等于


**语法:** `"@ge number"`


@ge 检查变量值（转换为整数后）是否大于等于指定的数值。常用于阈值检测场景，特别是在异常评分模式中判断累计评分是否达到阻断阈值。与 @le 配合可实现闭区间范围检测。


**示例:**


```apache
# 异常评分达到阻断阈值
SecRule TX:anomaly_score "@ge 5" \
    "id:1095,phase:2,deny,msg:'异常评分达到阈值: %{TX.anomaly_score}'"

# 检测大文件上传
SecRule FILES_COMBINED_SIZE "@ge 5242880" \
    "id:1096,phase:2,deny,msg:'上传文件总大小超过 5MB'"

# 检测暴力破解 (失败次数 >= 5)
SecRule IP:failed_login "@ge 5" \
    "id:1097,phase:2,deny,msg:'疑似暴力破解攻击'"
```


**参数类型:** `int`


**区分大小写:** 是
