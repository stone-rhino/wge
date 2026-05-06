+++
title = "severity"
weight = 4
+++

**描述:** 标志严重级别。

**语法:** `severity:('EMERGENCY|ALERT|CRITICAL|ERROR|WARNING|NOTICE|INFO|DEBUG'|[0-7])`

**区分大小写:** 是

既可使用英文关键词，也可以是使用对应的数字，其对应关系为：
- **0** - **EMERGENCY**
- **1** - **ALERT**
- **2** - **CRITICAL**
- **3** - **ERROR**
- **4** - **WARNING**
- **5** - **NOTICE**
- **6** - **INFO**
- **7** - **DEBUG**

**注意：**若使用英文，则需要注意大小写，关键字全为大写。

**示例:**

```apache
SecRule ARGS "@rx test" "id:1001,deny,severity:CRITICAL"
```