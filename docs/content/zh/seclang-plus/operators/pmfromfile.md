+++
title = "@pmFromFile"
weight = 11
+++

**描述:** 从文件加载关键字进行并行匹配


**语法:** `"@pmFromFile /path/to/keywords.txt"`


与 @pmf 功能完全相同。文件中每行一个关键字，以 # 开头的行将作为注释忽略。支持相对路径（相对于配置文件目录）和绝对路径。


**示例:**


```apache
# 从文件加载恶意 User-Agent 列表
SecRule REQUEST_HEADERS:User-Agent "@pmFromFile /etc/wge/bad-ua.txt" \
    "id:1001,phase:1,deny,msg:'Malicious User-Agent'"
```


**参数类型:** `string (文件路径)`
