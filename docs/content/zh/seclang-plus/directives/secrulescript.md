+++
title = "SecRuleScript"
weight = 47
+++

**描述:** 使用 Lua 脚本定义规则逻辑。


**语法:** `SecRuleScript LUA_PATH "ACTIONS"`


**区分大小写:** 是

**是否实现:** 否

此指令创建特殊规则，通过执行 Lua 脚本决定是否匹配。与 SecRule 的主要区别在于不存在目标和运算符。脚本可获取 WAF 上下文中的任意变量，并使用 Lua 运算符进行测试。所有 Lua 脚本在配置加载时编译并缓存至内存，若需重新加载脚本，必须重新加载整个配置。


**示例:**


```apache
SecRuleScript /etc/wge/scripts/custom_check.lua "id:5001,phase:2,deny,msg:'Script check failed'"
```
