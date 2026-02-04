+++
title = "@pm"
weight = 9
+++

**描述:** 使用 Aho-Corasick 算法进行高效的多模式并行匹配


**语法:** `"@pm keyword1 keyword2 keyword3 ..."`


@pm 操作符可以同时搜索多个关键字，使用空格分隔。相比多个 @rx 规则，@pm 在匹配大量关键字时性能更优。匹配不区分大小写。


**示例:**


```apache
# 检测常见 SQL 注入关键字
SecRule ARGS "@pm select union insert update delete" \
    "id:1001,phase:2,deny,msg:'SQL keyword detected'"

# 检测敏感文件访问
SecRule REQUEST_URI "@pm /etc/passwd /etc/shadow .htaccess" \
    "id:1002,phase:1,deny,msg:'Sensitive file access'"
```


**参数类型:** `string (空格分隔的关键字列表)`
