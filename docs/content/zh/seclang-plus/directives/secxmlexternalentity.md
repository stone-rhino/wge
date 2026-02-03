+++
title = "SecXmlExternalEntity"
weight = 25
+++

**描述:** 配置是否允许 XML 外部实体处理。

            
**语法:** `SecXmlExternalEntity On|Off`

            
**默认值:** Off

            
**区分大小写:** 是

            
            
                
            
            
XXE 攻击可能导致敏感文件泄露、服务器端请求伪造 (SSRF)、拒绝服务等安全问题。

            
**示例:**

            
```
SecXmlExternalEntity Off
```
