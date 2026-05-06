+++
title = "@pmf"
weight = 10
+++

**描述:** pmFromFile的简称

**语法:** `@pmf /path/to/keywords.txt`

**输入数据类型:** `string (文件路径)`

详细介绍见pmFromFile的描述。

**示例:**

```apache
# 从文件加载 SQL 注入关键字
SecRule ARGS "@pmf /etc/wge/sql-keywords.txt" \
    "id:1001,phase:2,deny,msg:'SQL keyword detected'"
```