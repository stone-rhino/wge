+++
title = "SecXmlExternalEntity"
weight = 25
+++

**Description:** Configure whether XML external entity processing is allowed.

**Syntax:** `SecXmlExternalEntity On|Off`

**Default:** Off

XXE attacks can lead to sensitive file disclosure, Server-Side Request Forgery (SSRF), denial of service, and other security issues.

**Example:**

```
SecXmlExternalEntity Off
```

**Case Sensitive:** Yes
