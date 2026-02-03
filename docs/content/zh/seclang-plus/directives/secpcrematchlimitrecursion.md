+++
title = "SecPcreMatchLimitRecursion"
weight = 13
+++

**描述:** 配置 PCRE 正则表达式递归深度限制。

            
**语法:** `SecPcreMatchLimitRecursion LIMIT`

            
**默认值:** 1000

            
**区分大小写:** 是

            
            
此指令限制正则表达式匹配时的递归调用深度，用于防止复杂正则表达式导致的栈溢出。当正则表达式包含嵌套的分组或回溯时，递归深度可能快速增加。若达到限制，匹配将终止并设置 MSC_PCRE_LIMITS_EXCEEDED 变量。

            
**示例:**

            
```
SecPcreMatchLimitRecursion 1000
```
