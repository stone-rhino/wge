+++
title = "SecPcreMatchLimit"
weight = 12
+++

**描述:** 配置 PCRE 正则表达式匹配的最大次数,防止 ReDoS 攻击。

            
**语法:** `SecPcreMatchLimit LIMIT`

            
**默认值:** 3000

            
**区分大小写:** 是

            
            
若配置的限制被超过，将设置变量 MSC_PCRE_LIMITS_EXCEEDED。

            
**示例:**

            
```
SecPcreMatchLimit 3000
```
