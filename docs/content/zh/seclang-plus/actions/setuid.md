+++
title = "setuid"
weight = 28
+++

**描述:** 使用传入参数初始化USER集合。

**语法:** `setuid:('string' | %{variable})`

**区分大小写:** 是

**是否实现:** 否

此动作支持不匹配时执行和无论是否匹配都执行。

此指令WGE可以正常解析，但是暂未实现该功能。

**示例:**

```apache
SecRule ARGS:username ".*" "phase:2,id:137,t:none,pass,capture,setuid:%{TX.0}"

# 匹配失败后执行
SecRule ARGS:username ".*" "phase:2,id:137,t:none,pass,capture,!setuid:%{TX.0}"

# 无论是否匹配成功都执行
SecRule ARGS:username ".*" "phase:2,id:137,t:none,pass,capture,*setuid:%{TX.0}"
```