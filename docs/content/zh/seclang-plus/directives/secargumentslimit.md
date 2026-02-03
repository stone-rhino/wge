+++
title = "SecArgumentsLimit"
weight = 10
+++

**描述:** 配置可接受的最大参数数量。

            
**语法:** `SecArgumentsLimit LIMIT`

            
**区分大小写:** 是

            
            
使用此设置时，建议配合规则检测相同整数值，达到上限时拒绝请求。例如：
若未配置匹配规则，攻击者可能通过将攻击有效载荷放置在超出限制的参数中规避检测。

            
**示例:**

            
```
SecArgumentsLimit 1000
```
