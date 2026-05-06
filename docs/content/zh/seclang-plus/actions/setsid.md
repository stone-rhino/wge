+++
title = "setsid"
weight = 29
+++

**描述:** 使用传入参数始化SESSION集合。

**语法:** `setsid:('string' | %{variable})`

**区分大小写:** 是

**是否实现:** 暂无

此动作支持不匹配时执行和无论是否匹配都执行。

此指令WGE可以正常解析，但是暂未实现该功能。

**示例:**

```apache
SecRule ARGS "@rx test" "id:1001,phase:2,setsid:%{REQUEST_COOKIES.PHPSESSID},status:403"

# 匹配失败后执行
SecRule ARGS "@rx test" "id:1001,phase:2,!setsid:%{REQUEST_COOKIES.PHPSESSID},status:403"

# 无论是否匹配成功都执行
SecRule ARGS "@rx test" "id:1001,phase:2,*setsid:%{REQUEST_COOKIES.PHPSESSID},status:403"
```