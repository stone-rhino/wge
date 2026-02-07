+++
title = "drop"
weight = 11
+++

**描述:** 丢弃连接

**语法:** `drop`

**区分大小写:** 是

在WGE内部关于 drop 和 deny 的处理逻辑是一致的，Modsecurity中是直接发送FIN包关闭TCP连接，这个功能需要使用WGE的连接器或者服务器判断WGE是否drop状态并做相应的操作。

**示例:**

```apache
SecRule ARGS "@rx test" "id:1001,drop,status:403"
```