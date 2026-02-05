+++
title = "ctl"
weight = 41
+++

**描述:** 修改运行时配置

**语法:** `ctl:directive=value`

ctl（控制）动作允许在运行时修改当前事务的处理指令。这使得规则行为能够根据请求特征动态变化。

可用选项：
- **ctl:auditEngine=On|Off|RelevantOnly** - 更改审计日志模式
- **ctl:auditLogParts=+/-PARTS** - 添加或移除审计日志部分
- **ctl:forceRequestBodyVariable=On|Off** - 强制填充 REQUEST_BODY 变量
- **ctl:requestBodyAccess=On|Off** - 启用/禁用请求体检查
- **ctl:requestBodyProcessor=URLENCODED|MULTIPART|XML|JSON** - 设置请求体处理器
- **ctl:ruleEngine=On|Off|DetectionOnly** - 更改规则引擎模式
- **ctl:ruleRemoveById=ID** - 按 ID 移除当前事务的规则
- **ctl:ruleRemoveByTag=TAG** - 按标签移除当前事务的规则
- **ctl:ruleRemoveTargetById=ID;VARIABLE** - 从规则中移除检测目标
- **ctl:ruleRemoveTargetByTag=TAG;VARIABLE** - 按标签从规则中移除检测目标

**示例:**

```apache
# 对静态文件禁用规则引擎
SecRule REQUEST_URI "\.(css|js|png|jpg|gif)$" \
    "id:100,phase:1,pass,nolog,ctl:ruleEngine=Off"

# 为 API 端点启用 JSON 请求体处理
SecRule REQUEST_URI "^/api/" \
    "id:101,phase:1,pass,nolog,ctl:requestBodyProcessor=JSON"

# 为可信路径移除特定规则
SecRule REQUEST_URI "^/trusted/" \
    "id:102,phase:1,pass,nolog,ctl:ruleRemoveById=942100"

# 为可疑请求添加审计日志部分
SecRule TX:suspicious "@eq 1" \
    "id:103,phase:5,pass,nolog,ctl:auditLogParts=+EKZ"
```

**参数类型:** `string`

**区分大小写:** 是
