+++
title = "@ge"
weight = 16
+++

**描述:** 大于等于


**语法:** `@ge string`

**输入数据类型:** `int`

@ ge 检查输入数据是否大于等于指定的数值。其内部实现与操作符eq差别不大，详细信息可见操作符 eq。

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