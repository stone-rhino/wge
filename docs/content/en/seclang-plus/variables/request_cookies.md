+++
title = "REQUEST_COOKIES"
weight = 10
+++

**Description:** Request cookies

**Syntax:** `REQUEST_COOKIES | REQUEST_COOKIES:CookieName`

REQUEST_COOKIES contains the values of all cookies in the request. Specific cookies can be accessed using REQUEST_COOKIES:name. Cookies are common attack vectors where attackers may inject malicious code or attempt session hijacking.

**Example:**

```apache
# Check all cookies for SQL injection
SecRule REQUEST_COOKIES "@detectSQLi" \
    "id:1013,phase:1,deny,msg:'SQL injection detected in cookie'"

# Validate session cookie format
SecRule REQUEST_COOKIES:SESSIONID "!@rx ^[a-zA-Z0-9]{32}$" \
    "id:1014,phase:1,deny,msg:'Invalid session cookie format'"
```
