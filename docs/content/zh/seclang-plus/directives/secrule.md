+++
title = "SecRule"
weight = 37
+++

**描述:** 定义一个安全规则。


**语法:** `SecRule VARIABLES "OPERATOR" "ACTIONS"`

**区分大小写:** 是

SecRule 是 SecLang 中最重要的指令,用于定义检测和响应安全威胁的规则。一个规则由三部分组成:


- **变量 (VARIABLES):** 指定要检查的数据源，如ARGS代表查询参数
- **操作符 (OPERATOR):** 定义匹配条件，包括正则匹配、大小写比较等
- **动作 (ACTIONS):** 包含phase、id等配置动作、转换函数和setvar等匹配后动作

一条SecRule的执行逻辑为：从变量 (VARIABLES)获取到被匹配的数据，将数据经过转换函数进行编解码操作（如 t:urlDecode 进行url解码），将解码后的数据进行指定匹配操作，匹配成功后再执行匹配后动作（如 setvar 等），若存在子规则，会继续执行子规则。


**基本示例:**


```apache
SecRule ARGS "@contains " \
    "id:1001,phase:2,deny,status:403,msg:'XSS Attack Detected'"
```



**多个变量示例:**


```apache
SecRule ARGS|REQUEST_HEADERS "@rx (?i:eval\s*\()" \
    "id:1002,phase:2,deny,status:403,msg:'Code Injection Detected'"
```



**使用转换示例:**


```apache
SecRule ARGS "@rx select.*from" \
    "id:1003,phase:2,t:lowercase,t:removeWhitespace,deny,msg:'SQL Injection'"
```
