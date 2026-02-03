+++
title = "SecResponseBodyAccess"
weight = 7
+++

**描述:** 配置是否允许 WAF 访问响应体内容。

            
**语法:** `SecResponseBodyAccess On|Off`

            
**默认值:** Off

            
**区分大小写:** 是

            
            
若需检查 HTML 响应并实现响应阻断，则必须使用此指令。可选值：

            
- **On:** 缓冲并检查响应体
- **Off:** 不缓冲响应体
**示例:**

            
```
SecResponseBodyAccess On
```
