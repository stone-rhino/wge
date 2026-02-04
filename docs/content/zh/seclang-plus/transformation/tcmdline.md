+++
title = "t:cmdline"
weight = 10
+++

**描述:** 命令行参数规范化


**语法:** `t:cmdline`


t:cmdline 将命令行参数规范化，用于检测命令注入攻击。在 Windows 和 Unix 系统中，命令可能通过不同方式转义（如使用引号、反斜杠、脱字符等）。此转换函数通过以下方式处理输入：


- 删除所有反斜杠 (\)
- 删除所有双引号 (")
- 删除所有单引号 (')
- 删除所有脱字符 (^)
- 删除 /之前的空格
- 删除 -之前的空格
- 压缩多余空白字符
**示例:**


```apache
# 检测命令注入
SecRule ARGS "@rx (cat|ls|wget|curl|nc|bash)" \
    "id:1,phase:2,t:cmdline,t:lowercase,deny,msg:'命令注入检测'"

# 原始输入: c^a"t /e't'c/p\asswd
# 处理后: cat /etc/passwd
```


**参数类型:** `无`


**区分大小写:** 是
