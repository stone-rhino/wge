+++
title = "SecRequestBodyAccess"
weight = 2
+++

**描述:** 配置是否允许 WAF 访问请求体内容。

            
**语法:** `SecRequestBodyAccess On|Off`

            
**默认值:** Off

            
**版本:** 1.0


            
                
            

            
**区分大小写:** 是

            
            
若需检查请求主体传输的数据（如 POST 参数），则必须使用此指令。为实现可靠阻塞，请求缓冲亦不可或缺。可选值：

            
- **On:** 缓冲并检查请求主体
- **Off:** 不缓冲请求主体
**示例:**

            
```
SecRequestBodyAccess On
```
