+++
title = "t:cmdline"
weight = 10
+++

**描述:** 对字符串进行命令行规范化。

此转换函数通过以下方式处理输入字符串：

- 删除所有斜杠 ``\``
- 删除所有双引号 ``"``
- 删除所有单引号 ``'``
- 删除所有脱字符 ``^``
- 删除斜杠 ``/`` 之前的空格
- 删除左括号 ``(`` 之前的空格
- 替换逗号 ``,`` 和 分号 ``;`` 为空格
- 压缩连续空格(包括制表符、换行符)为一个空格
- 字符转小写

**语法:** `t:cmdline`

**是否实现:** 是

**示例:**

```apache
# 检测命令注入
SecRule ARGS "@rx (cat|ls|wget|curl|nc|bash)" \
    "id:1,phase:2,t:cmdline,t:lowercase,deny,msg:'命令注入检测'"

# 原始输入: c^a"t /e't'c/p\asswd
# 处理后: cat /etc/passwd
```
