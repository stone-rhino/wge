+++
title = "xmlns"
weight = 46
+++

**描述:** 为 XPath 表达式定义 XML 命名空间

**语法:** `xmlns:prefix=namespaceURI`

xmlns 动作注册 XML 命名空间前缀，用于在检查 XML 请求体时使用 XPath 表达式。当 XML 文档使用命名空间且需要使用 XML 变量查询这些命名空间中的元素时，这是必需的。

注意：此动作与 XML 变量和基于 XPath 的操作符配合使用。

**示例:**

```apache
# 注册 SOAP 命名空间并检测特定 SOAP 动作
SecRule REQUEST_HEADERS:Content-Type "@contains text/xml" \
    "id:100,phase:1,pass,nolog,xmlns:soap=http://schemas.xmlsoap.org/soap/envelope/"

SecRule XML:/soap:Envelope/soap:Body/* "@rx malicious" \
    "id:101,phase:2,deny,status:403,msg:'恶意 SOAP 内容'"

# 多个命名空间注册
SecAction "id:200,phase:1,pass,nolog,\
    xmlns:wsse=http://docs.oasis-open.org/wss/2004/01/oasis-200401-wss-wssecurity-secext-1.0.xsd,\
    xmlns:wsu=http://docs.oasis-open.org/wss/2004/01/oasis-200401-wss-wssecurity-utility-1.0.xsd"

# 检查 WS-Security 头
SecRule XML:/soap:Envelope/soap:Header/wsse:Security "!@rx ." \
    "id:201,phase:2,deny,status:401,msg:'缺少 WS-Security 头'"
```

**参数类型:** `string`

**区分大小写:** 是
