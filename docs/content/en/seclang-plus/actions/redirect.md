+++
title = "redirect"
weight = 15
+++

**Description:** Redirect to specified URL

**Syntax:** `redirect:url`

The redirect action sends an HTTP redirect response to the client, directing them to a specified URL. This is useful for sending detected attackers to honeypots, warning pages, or blocked pages while providing a better user experience than a simple deny.

**Example:**

```apache
SecRule ARGS "@detectSQLi" "id:1001,redirect:https://example.com/blocked.html"
```

**Parameter Type:** `string`

**Case Sensitive:** Yes
