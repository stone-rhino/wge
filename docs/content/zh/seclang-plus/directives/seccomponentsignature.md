+++
title = "SecComponentSignature"
weight = 31
+++

**描述:** 添加组件签名信息到日志中。

            
**语法:** `SecComponentSignature "SIGNATURE"`

            
**区分大小写:** 是

            
            
此指令用于标识重要规则集的存在。完整签名将记录于事务审计日志中。

            
**示例:**

            
```
SecComponentSignature "OWASP_CRS/3.3.2"
```
