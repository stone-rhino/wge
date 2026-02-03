+++
title = "SecRule"
weight = 37
+++

**描述:** 定义一个安全规则。

            
**语法:** `SecRule VARIABLES "OPERATOR" "ACTIONS"`

            
**版本:** 1.0


            
SecRule 是 SecLang 中最重要的指令,用于定义检测和响应安全威胁的规则。一个规则由三部分组成:

            
                - **变量 (VARIABLES):** 指定要检查的数据源
- **操作符 (OPERATOR):** 定义匹配条件
- **动作 (ACTIONS):** 定义规则匹配后的行为


            
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

            
**区分大小写:** 是

            
```apache
SecRule ARGS "@rx select.*from" \
    "id:1003,phase:2,t:lowercase,t:removeWhitespace,deny,msg:'SQL Injection'"
```
