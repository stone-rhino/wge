+++
title = "tag"
weight = 5
+++

**描述:** 规则标签

**语法:** `tag:'tagname'`

**示例:**

```apache
SecRule ARGS "@rx test" "id:1001,deny,tag:'attack-sqli'"
```


**参数类型:** `string`


**区分大小写:** 是
