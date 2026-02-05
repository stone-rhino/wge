+++
title = "SecPcreMatchLimit"
weight = 12
+++

**描述:** 配置 PCRE 正则表达式匹配的最大次数,防止 ReDoS 攻击。


**语法:** `SecPcreMatchLimit LIMIT`


**默认值:** 0


**区分大小写:** 是


由于目前WGE默认使用的正则匹配库为re2，只有在re2编译正则失败的情况下才会退回使用pcre，所以此配置的影响不大。


**示例:**


```apache
SecPcreMatchLimit 3000
```
