+++
title = "@pmf"
weight = 10
+++

**描述:** 从文件加载关键字进行并行匹配（@pmFromFile 的别名）


**语法:** `"@pmf /path/to/keywords.txt"`


与 @pm 功能相同，但从外部文件加载关键字列表。文件中每行一个关键字，以 # 开头的行将作为注释忽略。适用于管理大量关键字的场景。


**示例:**


```apache
# 从文件加载 SQL 注入关键字
SecRule ARGS "@pmf /etc/wge/sql-keywords.txt" \
    "id:1001,phase:2,deny,msg:'SQL keyword detected'"
```


**参数类型:** `string (文件路径)`
