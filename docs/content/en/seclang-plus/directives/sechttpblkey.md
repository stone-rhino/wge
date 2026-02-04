+++
title = "SecHttpBlKey"
weight = 27
+++

**Description:** Configure the HTTP Blacklist (Project Honeypot) API key.

**Syntax:** `SecHttpBlKey API_KEY`

If the @rbl operator uses the dnsbl.httpbl.org RBL (http://www.projecthoneypot.org/httpbl_api.php), an API key must be provided. This key is bound to an individual user account and will be included in RBL DNS requests.

**Example:**

```
SecHttpBlKey abcdefghijkl
```

**Case Sensitive:** Yes
