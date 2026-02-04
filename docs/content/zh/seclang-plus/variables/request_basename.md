+++
title = "REQUEST_BASENAME"
weight = 18
+++

**描述:** 请求文件的基础名称


**语法:** `REQUEST_BASENAME`


REQUEST_BASENAME 仅包含请求路径中的文件名部分，不包括目录路径。例如，对于 /path/to/file.php，此变量的值为 file.php。用于检测特定文件名或文件扩展名。


**示例:**


```apache
# 阻止访问备份文件
SecRule REQUEST_BASENAME "@rx \.(bak|backup|old|orig)$" \
    "id:1026,phase:1,deny,msg:'禁止访问备份文件'"

# 阻止执行 shell 脚本
SecRule REQUEST_BASENAME "@endsWith .sh" \
    "id:1027,phase:1,deny,msg:'禁止执行 shell 脚本'"
```
