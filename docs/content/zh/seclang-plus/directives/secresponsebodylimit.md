+++
title = "SecResponseBodyLimit"
weight = 9
+++

**描述:** 配置响应体缓冲区的最大大小 (字节)。


**语法:** `SecResponseBodyLimit LIMIT`


**默认值:** 524288 (512KB)


**区分大小写:** 是

**是否实现:** 否

此指令WGE可以正常解析，但是暂未实现该功能。

**示例:**


```apache
SecResponseBodyLimit 524288
```
