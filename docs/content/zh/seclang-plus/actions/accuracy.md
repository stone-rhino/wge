+++
title = "accuracy"
weight = 8
+++

**描述：** 准确度级别 (1-9)

**语法：** `accuracy:[1-9]`


**区分大小写：** 是

**示例:**

```apache
SecRule ARGS "@rx test" "id:1001,deny,accuracy:8"
```