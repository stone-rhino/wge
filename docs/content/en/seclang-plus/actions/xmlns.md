+++
title = "xmlns"
weight = 46
+++

**Description:** Define XML namespace for XPath expressions

**Syntax:** `xmlns:prefix=namespaceURI`

The xmlns action registers an XML namespace prefix for use in XPath expressions when inspecting XML request bodies. This is required when the XML document uses namespaces and you need to query elements within those namespaces using the XML variable.

Note: This action is used in conjunction with the XML variable and XPath-based operators.

**Example:**

```apache
# Register SOAP namespace and detect specific SOAP actions
SecRule REQUEST_HEADERS:Content-Type "@contains text/xml" \
    "id:100,phase:1,pass,nolog,xmlns:soap=http://schemas.xmlsoap.org/soap/envelope/"

SecRule XML:/soap:Envelope/soap:Body/* "@rx malicious" \
    "id:101,phase:2,deny,status:403,msg:'Malicious SOAP content'"

# Multiple namespace registration
SecAction "id:200,phase:1,pass,nolog,\
    xmlns:wsse=http://docs.oasis-open.org/wss/2004/01/oasis-200401-wss-wssecurity-secext-1.0.xsd,\
    xmlns:wsu=http://docs.oasis-open.org/wss/2004/01/oasis-200401-wss-wssecurity-utility-1.0.xsd"

# Check for WS-Security headers
SecRule XML:/soap:Envelope/soap:Header/wsse:Security "!@rx ." \
    "id:201,phase:2,deny,status:401,msg:'Missing WS-Security header'"
```

**Parameter Type:** `string`

**Case Sensitive:** Yes
