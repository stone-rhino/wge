+++
title = "@endsWith"
weight = 4
+++

**描述:** 以指定字符串结尾


**语法:** `"@endsWith string"`


@endsWith 检查变量值是否以指定的字符串结尾。匹配区分大小写。相比使用 @rx "suffix$" 的正则表达式，@endsWith 在进行简单后缀检查时性能更优。常用于文件扩展名检查、路径验证等场景。


**示例:**


```apache
# 检测危险文件扩展名访问
SecRule REQUEST_FILENAME "@endsWith .bak" \
    "id:1074,phase:1,deny,msg:'禁止访问备份文件'"

# 检测配置文件访问
SecRule REQUEST_URI "@endsWith .conf" \
    "id:1075,phase:1,deny,msg:'禁止访问配置文件'"

# 检测 PHP 文件上传
SecRule FILES_NAMES "@endsWith .php" \
    "id:1076,phase:2,deny,msg:'禁止上传 PHP 文件'"

# 检测隐藏文件 (Unix 风格)
SecRule REQUEST_FILENAME "@endsWith /.htaccess" \
    "id:1077,phase:1,deny,msg:'禁止访问 .htaccess 文件'"
```


**参数类型:** `string`


**区分大小写:** 是
