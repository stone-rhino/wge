+++
title = "SecWebAppId"
weight = 30
+++

**Description:** Configure the web application identifier to distinguish between different applications.

**Syntax:** `SecWebAppId NAME`

The application namespace is used to prevent session ID and user ID collisions when multiple applications are deployed on the same server. Without this feature, session ID collisions may occur.

In the example configurations, SecWebAppId is used in conjunction with nginx server blocks. The configuration value is stored in the WEBAPPID variable.

**Example:**

```
SecWebAppId "MyWebApp"
```

**Case Sensitive:** Yes
