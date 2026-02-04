+++
title = "ARGS"
weight = 1
+++

**Description:** All request parameters (GET and POST)

**Syntax:** `ARGS | ARGS:Key`

**Example:**

```apache
SecRule ARGS:User-Agent "@rx box" "id:1001,deny,msg:'Test'"
```

RESTful API parsing examples:

1. JSON request body parsing (application/json)

**Request:**

```
POST /api/users HTTP/1.1
Content-Type: application/json

{
  "username": "admin",
  "password": "secret123",
  "profile": {
    "email": "admin@example.com",
    "role": "administrator"
  },
  "tags": ["vip", "test"]
}
```

**ARGS parsing result:**

```
ARGS:username = "admin"
ARGS:password = "secret123"
ARGS:profile.email = "admin@example.com"
ARGS:profile.role = "administrator"
ARGS:tags.0 = "vip"
ARGS:tags.1 = "test"
```

**Rule examples:**

```apache
# Detect sensitive fields in JSON
SecRule ARGS:password "@rx (?i)(password|secret)" \
    "id:1001,phase:2,deny,status:403,msg:'Sensitive data in password field'"

# Detect nested JSON fields
SecRule ARGS:profile.role "@streq administrator" \
    "id:1002,phase:2,log,msg:'Admin role detected'"
```

2. XML request body parsing (application/xml)

**Request:**

```
POST /api/order HTTP/1.1
Content-Type: application/xml

<order>
  <customer>
    <name>John Doe</name>
    <email>john@example.com</email>
  </customer>
  <items>
    <item>
      <name>Product A</name>
      <quantity>2</quantity>
    </item>
  </items>
</order>
```

**XML variable access (requires XPath):**

```apache
# Declare namespace if needed
SecRule REQUEST_HEADERS:Content-Type "@contains xml" \
    "id:2000,phase:1,pass,nolog,ctl:requestBodyProcessor=XML"

# Use XPath to access XML nodes
SecRule XML:/order/customer/name/text() "@rx admin" \
    "id:2001,phase:2,deny,msg:'Blocked admin in customer name'"

SecRule XML:/order/customer/@id "@eq 0" \
    "id:2002,phase:2,deny,msg:'Invalid customer ID'"

SecRule XML://item/@sku "@rx ^[A-Z]{3}[0-9]{3}$" \
    "id:2003,phase:2,pass,msg:'Valid SKU format'"
```

3. URL-encoded form (application/x-www-form-urlencoded)

**Request:**

```
POST /login HTTP/1.1
Content-Type: application/x-www-form-urlencoded

username=admin&password=secret%26123&remember=true
```

**ARGS parsing result:**

```
ARGS:username = "admin"
ARGS:password = "secret&123"  # URL decoded
ARGS:remember = "true"
```

**Rule examples:**

```apache
SecRule ARGS:username "@rx ^[a-zA-Z0-9_]{3,20}$" \
    "id:3001,phase:2,pass,nolog"

SecRule ARGS:username "!@rx ^[a-zA-Z0-9_]{3,20}$" \
    "id:3002,phase:2,deny,status:400,msg:'Invalid username format'"
```

4. Multipart form (multipart/form-data)

**Request:**

```
POST /upload HTTP/1.1
Content-Type: multipart/form-data; boundary=----WebKitFormBoundary

------WebKitFormBoundary
Content-Disposition: form-data; name="title"

My Document
------WebKitFormBoundary
Content-Disposition: form-data; name="file"; filename="report.pdf"
Content-Type: application/pdf

[binary content]
------WebKitFormBoundary--
```

**ARGS and FILES parsing result:**

```
ARGS:title = "My Document"
FILES:file = "report.pdf"
FILES_NAMES:file = "report.pdf"
FILES_SIZES:file = 102400
FILES_TMPNAMES:file = "/tmp/upload_abc123"
```

**Rule examples:**

```apache
# Check file extension
SecRule FILES_NAMES "@rx \.(php|exe|sh)$" \
    "id:4001,phase:2,deny,status:403,msg:'Dangerous file extension'"

# Limit file size (5MB)
SecRule FILES_SIZES "@gt 5242880" \
    "id:4002,phase:2,deny,status:413,msg:'File too large'"
```
