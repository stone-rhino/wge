+++
title = "phase"
weight = 2
+++

**描述:** 处理阶段 (1-5)


**语法:** `phase:1|2|3|4|5|request|response|logging`


phase 动作指定规则在哪个处理阶段执行。WGE 在处理 HTTP 事务时会按顺序执行五个阶段，每个阶段在特定数据可用时触发。选择正确的阶段对规则的有效性至关重要。


- **phase:1 (request)** - 请求头阶段：在接收完整请求头后立即执行，此时请求体尚未读取。用于早期决策，如 IP 黑名单、URL 检测。
- **phase:2 (request)** - 请求体阶段：在请求体完全接收并解析后执行。大多数应用层规则在此阶段运行，可访问 ARGS、REQUEST_BODY 等变量。
- **phase:3 (response)** - 响应头阶段：在接收后端响应头后执行，此时响应体尚未读取。用于检查响应状态码和响应头。
- **phase:4 (response)** - 响应体阶段：在响应体完全接收后执行。用于检测响应内容中的敏感数据泄露。
- **phase:5 (logging)** - 日志阶段：在响应发送完成后执行，无法阻止请求。仅用于日志记录和统计。
**示例:**


```apache
# 在请求头阶段检测 IP 黑名单
SecRule REMOTE_ADDR "@ipMatch 1.2.3.4" "id:1001,phase:1,deny,msg:'IP 黑名单'"

# 在请求体阶段检测 SQL 注入
SecRule ARGS "@detectSQLi" "id:1002,phase:2,deny,msg:'SQL 注入检测'"

# 在响应体阶段检测敏感数据泄露
SecRule RESPONSE_BODY "@rx \d{16}" "id:1003,phase:4,deny,msg:'信用卡号泄露'"
```


**参数类型:** `int 或 string`


**区分大小写:** 是
