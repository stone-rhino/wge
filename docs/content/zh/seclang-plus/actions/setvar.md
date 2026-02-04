+++
title = "setvar"
weight = 25
+++

**描述:** 设置变量


**语法:** `setvar:collection.key=value`


setvar 动作用于创建、修改或删除集合变量。这是实现异常评分、状态跟踪和规则间数据传递的核心动作。支持多种操作：


- **setvar:tx.key=value** - 设置变量为指定值
- **setvar:tx.key=+value** - 将变量增加指定值（数值累加）
- **setvar:tx.key=-value** - 将变量减少指定值
- **setvar:!tx.key** - 删除变量

可在单条规则中使用多个 setvar 动作。支持宏扩展，可使用 %{VARIABLE} 引用其他变量的值。


**示例:**


```apache
# 初始化异常评分
SecAction "id:1,phase:1,pass,nolog,setvar:tx.anomaly_score=0"

# 检测到攻击时增加评分
SecRule ARGS "@detectSQLi" \
    "id:2,phase:2,pass,setvar:tx.anomaly_score=+5,msg:'SQL 注入 +5'"

# 设置标志变量
SecRule REQUEST_URI "@beginsWith /api" \
    "id:3,phase:1,pass,nolog,setvar:tx.is_api=1"

# 使用宏扩展设置变量
SecRule REQUEST_HEADERS:User-Agent "@rx (bot|spider)" \
    "id:4,phase:1,pass,setvar:tx.client_type=%{MATCHED_VAR}"

# 删除变量
SecRule TX:temporary "@eq 1" \
    "id:5,phase:5,pass,nolog,setvar:!tx.temporary"
```


**参数类型:** `string`


**区分大小写:** 是
