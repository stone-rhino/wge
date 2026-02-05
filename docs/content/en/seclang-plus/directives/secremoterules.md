+++
title = "SecRemoteRules"
weight = 28
+++

**Description:** Load rule configuration from a remote HTTPS server.


**Case Sensitive:** Yes


**Implemented:** No


This directive allows dynamically loading rules from a remote server, facilitating centralized management and updates of security policies. The KEY parameter is used for authentication, allowing the target server to provide differentiated rule sets for different clients.


**Example:**


```apache
SecRemoteRules some-key https://example.com/rules.conf
```
