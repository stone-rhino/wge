+++
title = "@beginsWith"
weight = 3
+++

**描述:** 以指定字符串开头


**语法:** `"@beginsWith string"`


@beginsWith 检查变量值是否以指定的字符串开头。匹配区分大小写。相比使用 @rx "^prefix" 的正则表达式，@beginsWith 在进行简单前缀检查时性能更优，因为避免了正则引擎的开销。常用于 URL 路径检查、协议验证等场景。


**示例:**


```apache
# 检测管理后台访问
SecRule REQUEST_URI "@beginsWith /admin" \
    "id:1070,phase:1,deny,msg:'未授权访问管理路径'"

# 检测 API 路径并标记
SecRule REQUEST_URI "@beginsWith /api/v" \
    "id:1071,phase:1,pass,nolog,setvar:tx.is_api_request=1"

# 检测危险协议 (如 javascript:)
SecRule ARGS "@beginsWith javascript:" \
    "id:1072,phase:2,deny,msg:'检测到 JavaScript 协议注入'"

# 检测 Base64 编码数据
SecRule REQUEST_BODY "@beginsWith data:image" \
    "id:1073,phase:2,pass,nolog,setvar:tx.has_base64_image=1"
```


**参数类型:** `string`


**区分大小写:** 是
