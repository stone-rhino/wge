+++
title = "XML"
weight = 73
+++

**Description:** XML data

**Syntax:** `XML:/xpath/expression`

The XML variable is used to access specific nodes in XML request bodies via XPath expressions. The XML parser must first be enabled via ctl:requestBodyProcessor=XML. Supports full XPath syntax, including attribute access, text node extraction, etc.

**Example:**

```apache
# Enable XML parsing
SecRule REQUEST_HEADERS:Content-Type "@contains xml" \
    "id:1048,phase:1,pass,nolog,ctl:requestBodyProcessor=XML"

# Check XML nodes using XPath
SecRule XML:/root/user/name/text() "@detectSQLi" \
    "id:1049,phase:2,deny,msg:'SQL injection detected in XML node'"
```
