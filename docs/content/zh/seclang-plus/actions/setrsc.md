+++
title = "setrsc"
weight = 30
+++

**描述:** 使用作为传入参数提供的密钥初始化RESOURCE集合的特殊用途操作。

**语法:** `setrsc:('string' | %{variable})`

**区分大小写:** 是

**是否实现:** 否

此动作支持不匹配时执行和无论是否匹配都执行。

此指令WGE可以正常解析，但是暂未实现该功能。

**示例:**

```apache
SecRule ARGS "@rx test" "id:1001,setrsc:'abcd1234',status:403"

# 匹配失败后执行
SecRule ARGS "@rx test" "id:1002,!setrsc:'abcd1234',status:403"

# 无论是否匹配成功都执行
SecRule ARGS "@rx test" "id:1003,*setrsc:'abcd1234',status:403"
```