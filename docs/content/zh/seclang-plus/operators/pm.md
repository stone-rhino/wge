+++
title = "@pm"
weight = 9
+++

**描述:** 进行大小写不区分的字符串匹配

**语法:** `@pm string1 string2 string3 ...`

**输入数据类型:** `string (空格分隔的关键字列表)`

@pm 操作符可以同时搜索多个关键字，使用空格分隔。相比多个 @rx 规则，@pm 在匹配大量关键字时性能更优，匹配不区分大小写。

相比于Modsecurity使用的 Aho-Corasick 匹配算法，WGE使用的是hyperscan的多模匹配。

**示例:**
```apache
# 检测常见 SQL 注入关键字
SecRule ARGS "@pm select union insert update delete" \
    "id:1001,phase:2,deny,msg:'SQL keyword detected'"

# 检测敏感文件访问
SecRule REQUEST_URI "@pm /etc/passwd /etc/shadow .htaccess" \
    "id:1002,phase:1,deny,msg:'Sensitive file access'"
```