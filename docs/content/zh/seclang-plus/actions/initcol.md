+++
title = "initcol"
weight = 31
+++

**描述:** 初始化持久化集合


**语法:** `initcol:collection.string=key`


initcol 动作用于初始化持久化集合（IP、SESSION、USER、RESOURCE、GLOBAL）。持久化集合允许在多个请求之间保存和共享数据，是实现 IP 频率限制、会话跟踪、用户行为分析等功能的基础。


**这里的collection可选:**

- **IP** - 基于客户端 IP 地址的持久化存储
- **SESSION** - 基于会话 ID 的持久化存储
- **USER** - 基于用户 ID 的持久化存储
- **RESOURCE** - 基于资源标识的持久化存储
- **GLOBAL** - 全局持久化存储

但是目前WGE并未实现持久化，也就是说其和 setvar 动作设置的变量没有什么区别。

**区分大小写:** 是

**注意：** 此动作允许控制规则匹配失败后匹配（而不是匹配成功后）。

**示例:**

```apache
# 初始化 IP 集合用于频率限制
SecAction "id:1,phase:1,pass,nolog,initcol:IP=%{REMOTE_ADDR}"

# 规则匹配失败后执行initcol
SecRule ARGS "@rx admin" "id:1,phase:1.pass,nolog,!initcol:IP=%{REMOTE_ADDR}"

# 规则无论是否匹配都执行initcol
SecRule ARGS "@rx admin" "id:1,phase:1.pass,nolog,*initcol:IP=%{REMOTE_ADDR}"
```